#pragma once

#include <opencv2/opencv.hpp>

/**
 * @file detector.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @class goliath::vision::Detector
     * @brief Base class of detectors that take an input and detect a vector of shapes, colors or sizes.
     */
    class Detector {
    public:
        /**
         * @param input Input Image
         */
        explicit Detector(const cv::Mat &input);
        /**
         * @param other Copy constructor
         */
        explicit Detector(const Detector &other);

        virtual ~Detector();

        /**
         * @return A vector of vectors representing different objects that can be referenced from other detectors or
         * commands.
         * It is abstract and can represent anything the implementation desires.
         */
        virtual std::vector<cv::Vec4d> detect() const = 0;

        /**
         * @brief Update input image
         * @param input New input image
         */
        void update(cv::Mat &input);

    protected:
        cv::Mat input;

    };
}
