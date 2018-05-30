#pragma once

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
     * @class goliath::vision::CannyProcessor
     * @brief Processor that returns a canny image of the input
     */
    class CannyProcessor : public Processor {
    public:
        /**
         * @param input Source image
         * @param minLowThreshold Canny min threshold
         * @param maxLowThreshold Canny max threshold
         */
        explicit CannyProcessor(const cv::Mat &input, int minLowThreshold, int maxLowThreshold);
        explicit CannyProcessor(Processor &other, int minLowThreshold, int maxLowThreshold);

        /**
         * @return Canny image
         */
        cv::Mat process() const override;

    private:
        int minLowThreshold, maxLowThreshold;
    };
}
