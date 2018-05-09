#pragma once

#include <opencv2/opencv.hpp>

#include "processor.h"

/**
 * @file color_processor.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @class goliath::vision::color_processor
     * @brief Processor that returns an image with only colors within a specific range
     */
    class color_processor : public processor {
    public:
        /**
         * @param input_bgr Input image
         * @param min Minimum color hsv range
         * @param max Maximum color hsv range
         */
        color_processor(const cv::Mat &input_bgr, const cv::Scalar &min, const cv::Scalar &max);
        color_processor(processor& other, const cv::Scalar &min, const cv::Scalar &max);

        /**
         * @return Black/white image from input with colors within a specific range
         */
        cv::Mat process() const override;

    private:
        const cv::Scalar min;
        const cv::Scalar max;
    };
}