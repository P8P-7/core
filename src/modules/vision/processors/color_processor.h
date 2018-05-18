#pragma once

#include "processor.h"

/**
 * @file color_processor.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @class goliath::vision::ColorProcessor
     * @brief Processor that returns an image with only colors within a specific range
     */
    class ColorProcessor : public Processor {
    public:
        /**
         * @param inputBgr Input image
         * @param min Minimum color hsv range
         * @param max Maximum color hsv range
         */
        ColorProcessor(const cv::Mat &inputBgr, const cv::Scalar &min, const cv::Scalar &max);

        ColorProcessor(Processor& other, const cv::Scalar &min, const cv::Scalar &max);

        /**
         * @return Black/white image from input with colors within a specific range
         */
        cv::Mat process() const override;

    private:
        const cv::Scalar min;
        const cv::Scalar max;
    };
}
