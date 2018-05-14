#pragma once

#include "../util/vision_error.h"
#include "processor.h"

/**
 * @file roi_processor.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @brief Takes an input and returns a ROI selection from that image
     */
    class roi_processor : public processor {
    public:
        /**
         * @param input Input image
         * @param x Top left of ROI X
         * @param y Top left of ROI Y
         * @param w Width of ROI
         * @param h Height of ROI
         */
        roi_processor(const cv::Mat &input, int x, int y, int w, int h);
        roi_processor(processor &other, int x, int y, int w, int h);

        /**
         * @return ROI Selection of the input
         */
        cv::Mat process() const override;

    private:
        const int x, y, w, h;
    };
}
