#pragma once

#include <opencv2/opencv.hpp>

namespace goliath::vision {
    class detector {
    public:
        explicit detector(const cv::Mat &input);

        virtual std::vector<cv::Vec4i> detect() const = 0;

    protected:
        const cv::Mat input;

    };
}
