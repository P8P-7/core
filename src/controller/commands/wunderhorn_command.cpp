#include "wunderhorn_command.h"

#include <vector>
#include <chrono>
#include <thread>
#include <boost/log/trivial.hpp>

using namespace goliath::handles;
using namespace goliath;

commands::WunderhornCommand::WunderhornCommand(const size_t& id)
    : Command(id, {HANDLE_CAM}) {
}

void commands::WunderhornCommand::execute(const HandleMap& handles, const CommandMessage& message) {
    vision::Webcam webcam = std::static_pointer_cast<WebcamHandle>(handles[HANDLE_CAM])->getDevice();
    vision::FollowLineDetector followLineDetector(webcam.getFrame(), 4, 80, 20, 20, 10, 10000);

    follow_line(followLineDetector, webcam);

    vision::ColorRegionDetector colorRegionDetector(webcam.getFrame(), 0, 0, 0, 0);
    while (true) {
        if (isInterrupted()) {
            return;
        }

        int detected;

        do {
            detected = static_cast<int>(colorRegionDetector.detect()[0][0]);
        } while (detected == -1);

        if(detected == 1) {
            break;
        }

        cv::Mat new_frame = webcam.getFrame();
        followLineDetector.update(new_frame);
    }

    std::this_thread::sleep_for(std::chrono::seconds(30));

    follow_line(followLineDetector, webcam);

    handles[HANDLE_CAM]->unlock();
}

void commands::WunderhornCommand::follow_line(vision::FollowLineDetector& followLineDetector, vision::Webcam &camera) {
    std::vector<cv::Vec4d> lines = followLineDetector.detect();
    int noLinesCount = 0;

    while (noLinesCount < 20) {
        if (isInterrupted()) {
            return;
        }

        if (lines[0][0] == vision::FollowLineDirection::NO_LINE) {
            noLinesCount++;
        }

        // TODO: Implement followning line movement here


        cv::Mat new_frame = camera.getFrame();
        followLineDetector.update(new_frame);
    }
}
