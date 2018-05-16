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
     * @enum goliath::vision::follow_line_direction
     * @brief Represents a direction the robot should go in, outputted from the follow_line_detector.
     */
    enum follow_line_direction {
        ON_COURSE = 0, /**< Robot should continue going straight */
        LEFT = 1, /**< Robot should go left */
        RIGHT = 2, /**< Robot should go right */
        NO_LINE = 3 /**< No single line detected */
    };

    /**
     * @class goliath::vision::follow_line_detector
     * @brief Takes in an input image, splits it up into "boxes". Within those boxes it checks if there is a single
     * contour, if so take the center of that contour and compare it to the center of the box. This will return an
     * offset. Take the average of all offsets to determine if the robot should go straight, left or right. The second
     * value will be the distance from the edge of the box (percentage) the robot will have to turn.
     */
    class follow_line_detector : public detector {
    public:
        /**
         * @param input Input Image
         * @param boxes Amount of boxes to create a ROI from
         * @param box_height The height of each of the boxes
         * @param boxes_bottom_margin Margin to start bottom-most box from
         * @param boxes_horizontal_margin Margins on both sides of the boxes
         * @param min_contour_area Minimal area of the contour within a box to be considered part of a line
         * @param max_contour_area Maximal area of the contour within a box to be considered part of a line
         */
        follow_line_detector(const cv::Mat& input, int boxes, int box_height, int boxes_bottom_margin,
                             int boxes_horizontal_margin, double min_contour_area, double max_contour_area);

        /**
         * @brief Copy constructor
         */
        follow_line_detector(const follow_line_detector& other);

        /**
         * @return Value [0][0] contains whether the robot should continue going straight, or go left/right. This value
         * is defined in the follow_line_direction enum. Value [0][1] contains the percentage from the edge of the
         * current box the camera is located. Can be used to determine the strength of the caterpillars. [0][2] Contains
         * the box used from the bottom up. [1] Contains the box represented as a Vec4b.
         */
        std::vector<cv::Vec4d> detect() const override;

    private:
        const int boxes, box_height, boxes_bottom_margin, boxes_horizontal_margin;
        const double min_contour_area, max_contour_area;
    };
}
