#pragma once

#include <algorithm>

#include "detector.h"
#include "../processors/canny_processor.h"

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
    class LineDetector : public Detector {
    public:
        LineDetector(const cv::Mat& input, double rho, double theta, int threshold, double minLineLength,
                      double maxLineGap);

        /**
         * @param input Input image
         * @param threshold Variable that changes how straight a line must be to be detected.
         * @param minLineLength Minimum length of the line
         * @param maxLineGap Maximum gap (pixels) between points on a line
         */
        LineDetector(const cv::Mat& input, int threshold, double minLineLength, double maxLineGap);

        LineDetector(const LineDetector& other);

        /**
         * @return Vector of all detected lines
         */
        std::vector<cv::Vec4d> detect() const override;

        /**
         * @return The longest lines in detected lines
         */
        std::vector<cv::Vec4d> longestLines(std::shared_ptr<Detector> detector) const;
        std::vector<cv::Vec4d> longestLines();


    protected:
        const int threshold;
        const double rho, theta, minLineLength, maxLineGap;
    };
}
