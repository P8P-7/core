#pragma once

#include <cmath>

#include "detector.h"
#include "../util/vision_error.h"
#include "../processors/roi_processor.h"

namespace goliath::vision {
    enum class follow_line_direction {
        ON_COURSE = 0,
        LEFT = 1,
        RIGHT = 2,
        ERROR = 3
    };

    class follow_line_detector : public detector {
    public:
        /**
         * @param input Input Image
         * @param boxes Amount of ROI's to check
         */
        follow_line_detector(const cv::Mat &input, int boxes, int box_height, int boxes_bottom_padding,
                             int boxes_horizontal_padding);
        follow_line_detector(const follow_line_detector &other, int boxes, int box_height, int boxes_bottom_padding,
                             int boxes_horizontal_padding);

        /**
         * @return Only value [0] has values. Containing a offset from the center for each of the boxes
         */
        std::vector<cv::Vec4d> detect() const override;

    private:
        const int boxes, box_height, boxes_bottom_padding, boxes_horizontal_padding;
    };
}
