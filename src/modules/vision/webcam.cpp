#include <goliath/webcam.h>

using namespace goliath::vision;

void webcam::get_frame(cv::Mat &frame) {
    if(cap.isOpened())
        cap >> frame;

    if(color)
        cv::cvtColor(frame, frame, color);
}