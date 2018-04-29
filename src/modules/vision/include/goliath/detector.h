#pragma once

#include <opencv2/opencv.hpp>

namespace goliath::vision {
    class detector {
    public:
        detector(const cv::Mat &input)
            : input(input) {}

        virtual cv::Mat detect() const = 0;

    protected:
        const cv::Mat input;
    };
}