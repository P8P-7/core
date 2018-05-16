#pragma once

#include <cmath>

#include "detector.h"
#include "../util/vision_error.h"
#include "../util/vision_utilities.h"
#include "../processors/roi_processor.h"

/**
 * @file follow_line_detector.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @enum goliath::vision::FollowLineDirection
     * @brief Represents a direction the robot should go in, outputted from the follow_line_detector.
     */
    enum FollowLineDirection {
        ON_COURSE = 0, /**< Robot should continue going straight */
        LEFT = 1, /**< Robot should go left */
        RIGHT = 2, /**< Robot should go right */
        NO_LINE = 3 /**< No single line detected */
    };

    /**
     * @class goliath::vision::FollowLineDetector
     * @brief Takes in an input image, splits it up into "boxes". Within those boxes it checks if there is a single
     * contour, if so take the center of that contour and compare it to the center of the box. This will return an
     * offset. Take the average of all offsets to determine if the robot should go straight, left or right. The second
     * value will be the distance from the edge of the box (percentage) the robot will have to turn.
     */
    class FollowLineDetector : public Detector {
    public:
        /**
         * @param input Input Image
         * @param boxes Amount of boxes to create a ROI from
         * @param boxHeight The height of each of the boxes
         * @param boxesBottomMargin Margin to start bottom-most box from
         * @param boxesHorizontalMargin Margins on both sides of the boxes
         * @param minContourArea Minimal area of the contour within a box to be considered part of a line
         * @param maxContourArea Maximal area of the contour within a box to be considered part of a line
         */
        FollowLineDetector(const cv::Mat& input, int boxes, int boxHeight, int boxesBottomMargin,
                             int boxesHorizontalMargin, double minContourArea, double maxContourArea);

        /**
         * @brief Copy constructor
         */
        FollowLineDetector(const FollowLineDetector& other);

        /**
         * @return Value [0][0] contains whether the robot should continue going straight, or go left/right. This value
         * is defined in the follow_line_direction enum. Value [0][1] contains the percentage from the edge of the
         * current box the camera is located. Can be used to determine the strength of the caterpillars. [0][2] Contains
         * the box used from the bottom up. [1] Contains the box represented as a Vec4b.
         */
        std::vector<cv::Vec4d> detect() const override;

    private:
        const int boxes, boxHeight, boxesBottomMargin, boxesHorizontalMargin;
        const double minContourArea, maxContourArea;
    };
}
