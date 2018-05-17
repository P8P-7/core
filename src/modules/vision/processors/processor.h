#pragma once

#include <opencv2/opencv.hpp>

/**
 * @file processor.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @class goliath::vision::processor
     * @brief Base class of processors that take in an input and manipulate in some way returning the output from the
     * virtual @see goliath::vision::processor#process method.
     */
    class Processor {
    public:
        /**
         * @param input Input image
         */
        explicit Processor(const cv::Mat& input);

        virtual ~Processor();

        /**
         * @return Output image
         */
        virtual cv::Mat process() const = 0;

    protected:
        /**
         * @brief Input image
         */
        cv::Mat input;
    };
}
