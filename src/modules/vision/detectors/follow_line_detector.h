#pragma once

#include <cmath>

#include "detector.h"
#include "../util/vision_error.h"
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
    enum class follow_line_direction {
        ON_COURSE = 0,
        LEFT = 1,
        RIGHT = 2,
        ERROR = 3
    };

    /**
     * @class goliath::vision::follow_line_detector
     * @brief Takes in an input image, splits it up into "boxes". Within those boxes it checks if there is a single
     * contour, if so take the center of that contour and compare it to the center of the box. This will return an
     * offset. Take the average of all offsets to determine if the robot should go straight, left or right.
     */
    class follow_line_detector : public detector {
    public:
         /**
          * @param input Input Image
          * @param boxes Amount of boxes to create a ROI from
          * @param box_height The height of each of the boxes
          * @param boxes_bottom_margin Margin to start bottom-most box from
          * @param boxes_horizontal_margin Margins on both sides of the boxes
          */
        follow_line_detector(const cv::Mat &input, int boxes, int box_height, int boxes_bottom_margin,
                             int boxes_horizontal_margin);
        /**
         * @brief Copy constructor
         */
        follow_line_detector(const follow_line_detector &other);

        /**
         * @return Only value [0][0] has a value. Containing a value whether the robot should continue going straight,
         * or go left/right. This value is defined in the follow_line_direction enum
         */
        std::vector<cv::Vec4d> detect() const override;

    private:
        const int boxes, box_height, boxes_bottom_margin, boxes_horizontal_margin;
    };
}
