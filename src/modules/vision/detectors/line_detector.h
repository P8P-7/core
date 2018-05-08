#pragma once

#include "detector.h"
#include "../processors/line_processor.h"

/**
 * @file line_detector.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @brief Default Rho
     */
    const double DEFAULT_LINE_DETECTOR_RHO = 1;
    /**
     * @brief Default Theta
     */
    const double DEFAULT_LINE_DETECTOR_THETA = CV_PI / 180;

    /**
     * @class goliath::vision::line_detector
     * @brief Takes in an image and detects all lines in the image
     */
    class line_detector : public detector {
    public:
        line_detector(const cv::Mat &input, double rho, double theta, int threshold, double min_line_length, double max_line_gap);
        /**
         * @param input Input image
         * @param threshold Variable that changes how straight a line must be to be detected.
         * @param min_line_length Minimum length of the line
         * @param max_line_gap Maximum gap (pixels) between points on a line
         */
        line_detector(const cv::Mat &input, int threshold, double min_line_length, double max_line_gap);

        /**
         * @return Vector of all detected lines
         */
        std::vector<cv::Vec4d> detect() const override;

    private:
        int threshold;
        double rho, theta, min_line_length, max_line_gap;
    };
}