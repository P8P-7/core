#include "webcam.h"

using namespace goliath::vision;

webcam::webcam(const int cap_id)
        : cap_id(cap_id), cap(cap_id) {
}

webcam::webcam(const int cap_id, const int color)
        : cap_id(cap_id), cap(cap_id), color(color) {
}

webcam::~webcam() {
    cap.release();
}

void webcam::get_frame(cv::Mat &frame) {
    if(cap.isOpened()) {
        cap >> frame;
    }

    if(color.is_initialized()) {
        cv::cvtColor(frame, frame, color.get());
    }
}

cv::Mat webcam::get_frame() {
    cv::Mat mat;
    get_frame(mat);

    return mat;
}