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
     * virtual process() method.
     */
    class processor {
    public:
        /**
         * @param input Input image
         */
        explicit processor(const cv::Mat &input);

        /**
         * @return Output image
         */
        virtual cv::Mat process() const = 0;

    protected:
        /**
         * @brief Input image
         */
        const cv::Mat input;
    };
}