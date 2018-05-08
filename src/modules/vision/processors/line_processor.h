#pragma once

#include <opencv2/imgproc.hpp>

#include "processor.h"

/**
 * @file line_processor.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @class goliath::vision::line_processor
     * @brief Processor that returns a canny image of the input
     */
    class line_processor : public processor {
    public:
        explicit line_processor(const cv::Mat &input);
        explicit line_processor(processor &other);

        /**
         * @return Canny image
         */
        cv::Mat process() const override;
    };
}