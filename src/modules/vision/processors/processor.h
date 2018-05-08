#pragma once

#include <opencv2/opencv.hpp>

namespace goliath::vision {
    class processor {
    public:
        explicit processor(const cv::Mat &input);

        virtual cv::Mat process() const = 0;

    protected:
        const cv::Mat input;
    };
}