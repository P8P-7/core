#pragma once

#include <opencv2/opencv.hpp>

#include "detector.h"

namespace goliath::vision {
    class color_detector : public detector {
    public:
        color_detector(const cv::Mat &input_hsv, const cv::Scalar min, const cv::Scalar max)
            : detector(input_hsv), min(min), max(max) {}

        // Detector chaining
        color_detector(detector& other, const cv::Scalar min, const cv::Scalar max)
            : detector(other.detect()), min(min), max(max) {}

        cv::Mat detect() const override;

    private:
        const cv::Mat input;
        const cv::Scalar min;
        const cv::Scalar max;
    };
}