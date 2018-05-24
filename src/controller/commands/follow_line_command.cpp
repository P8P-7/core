#include "follow_line_command.h"

#include <vector>
#include <chrono>
#include <thread>
#include <boost/log/trivial.hpp>

using namespace goliath::handles;
using namespace goliath;

commands::FollowLineCommand::FollowLineCommand(const size_t& id, const boost::property_tree::ptree& config)
    : Command(id, {HANDLE_CAM}, config) {
}

void commands::FollowLineCommand::execute(const HandleMap& handles, const CommandMessage& message) {
    vision::Webcam webcam = std::static_pointer_cast<WebcamHandle>(handles[HANDLE_CAM])->getDevice();
    vision::FollowLineDetector followLineDetector(webcam.getFrame(), 4, 80, 20, 20, 10, 10000);

    follow_line(followLineDetector, webcam);

    vision::ColorRegionDetector colorRegionDetector(webcam.getFrame(), config.get<int>("area_min_h"),
                                                    config.get<int>("area_max_h"), config.get<int>("area_min_s"),
                                                    config.get<int>("area_max_s"));
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

void commands::FollowLineCommand::follow_line(vision::FollowLineDetector& followLineDetector, vision::Webcam &camera) {
    std::vector<cv::Vec4d> lines = followLineDetector.detect();
    int noLinesCount = 0;

    while (noLinesCount < config.get<int>("max_line_errors")) {
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
