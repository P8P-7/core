#pragma once

#include <opencv2/opencv.hpp>
#include <boost/optional.hpp>

namespace goliath::vision {
    class webcam {
    public:
        explicit webcam(const int cap_id);
        webcam(const int cap_id, const int color);
        ~webcam();

        void get_frame(cv::Mat& frame);
        cv::Mat get_frame();

    private:
        const int cap_id;
        boost::optional<const int> color;

        cv::VideoCapture cap;
    };
}