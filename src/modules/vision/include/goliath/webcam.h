#pragma once

#include <opencv2/opencv.hpp>

namespace goliath::vision {
    class webcam {
    public:
        webcam(const int cap_id, const int color = 0)
            : cap_id(cap_id), cap(cap_id), color(color) {}

        void get_frame(cv::Mat& frame);

    private:
        const int cap_id;
        const int color;

        cv::VideoCapture cap;
    };
}