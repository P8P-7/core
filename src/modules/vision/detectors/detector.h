#pragma once

#include <boost/variant.hpp>
#include <opencv2/opencv.hpp>

/**
 * @file detector.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @class goliath::vision::detector
     * @brief Base class of detectors that take an input and detect a vector of shapes, colors or sizes.
     */
    class detector {
    public:
        /**
         * @param input Input Image
         */
        explicit detector(const cv::Mat &input);
        /**
         * @param other Copy constructor
         */
        explicit detector(const detector &other);

        virtual ~detector();

        /**
         * @return A vector of vectors representing different objects that can be referenced from other detectors or
         * commands.
         * It is abstract and can represent anything the implementation desires.
         */
        virtual std::vector<cv::Vec4d> detect() const = 0;

    protected:
        cv::Mat input;

    };
}
