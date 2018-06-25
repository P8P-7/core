#include <goliath/vision/webcam.h>

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

void Webcam::getFrame(cv::Mat& frame) {
    if (cap.isOpened()) {
        cap >> frame;
        cv::flip(frame,frame,-1);
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
