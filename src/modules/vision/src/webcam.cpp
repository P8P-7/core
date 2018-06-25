#include <goliath/vision/webcam.h>
#include <goliath/vision.h>

using namespace goliath::vision;

Webcam::Webcam(const int capId)
    : capId(capId), cap(capId) {
}

Webcam::Webcam(const int capId, const int color)
    : capId(capId), cap(capId), color(color) {
}

Webcam::~Webcam() {
    cap.release();
}

void Webcam::getFrame(cv::Mat &frame) {
    if (cap.isOpened()) {
        cap >> frame;
        cv::flip(frame, frame, -1);
    }

    if (color.is_initialized()) {
        cv::cvtColor(frame, frame, color.get());
    }
}

cv::Mat Webcam::getFrame() {
    cv::Mat mat;
    getFrame(mat);

    return mat;
}

void Webcam::getRoiFrame(cv::Mat &frame, const vision::RoiProcessor &roiProcessor) {
    RoiProcessor newProcessor(getFrame(), roiProcessor.getX(), roiProcessor.getY(), roiProcessor.getWidth(),
                              roiProcessor.getHeight());
    frame = newProcessor.process();
}

cv::Mat Webcam::getRoiFrame(const RoiProcessor &roiProcessor) {
    cv::Mat mat;
    getRoiFrame(mat, roiProcessor);
    return mat;
}
