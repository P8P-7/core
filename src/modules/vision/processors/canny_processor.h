#pragma once

#include <opencv2/imgproc.hpp>

#include "processor.h"

/**
 * @file canny_processor.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {

    /**
     * @brief Default kernel size
     */
    const int DEFAULT_CANNY_KERNEL = 3;

    /**
     * @class goliath::vision::canny_processor
     * @brief Processor that returns a canny image of the input
     */
    class canny_processor : public processor {
    public:
        /**
         * @param input Source image
         * @param min_low_treshold Canny min threshold
         * @param max_low_treshold Canny max threshold
         */
        explicit canny_processor(const cv::Mat &input, int min_low_threshold, int max_low_threshold);
        explicit canny_processor(processor &other);

        /**
         * @return Canny image
         */
        cv::Mat process() const override;

    private:
        int min_low_threshold, max_low_threshold;
    };
}