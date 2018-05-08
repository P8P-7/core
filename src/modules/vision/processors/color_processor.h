#pragma once

#include <opencv2/opencv.hpp>

#include "processor.h"

namespace goliath::vision {
    class color_processor : public processor {
    public:
        color_processor(const cv::Mat &input_hsv, const cv::Scalar min, const cv::Scalar max);

        // Detector chaining
        color_processor(processor& other, const cv::Scalar min, const cv::Scalar max);

        cv::Mat process() const override;

    private:
        const cv::Mat input;
        const cv::Scalar min;
        const cv::Scalar max;
    };
}