#pragma once

#include "processor.h"

/**
 * @file roi_processor.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @brief Takes an input and returns a ROI selection from that image
     */
    class RoiProcessor : public Processor {
    public:
        /**
         * @param input Input image
         * @param x Top left of ROI X
         * @param y Top left of ROI Y
         * @param w Width of ROI
         * @param h Height of ROI
         */
        RoiProcessor(const cv::Mat &input, int x, int y, int w, int h);

        RoiProcessor(Processor &other, int x, int y, int w, int h);

        /**
         * @return ROI Selection of the input
         */
        cv::Mat process() const override;

        const int getX() const;
        const int getY() const;
        const int getWidth() const;
        const int getHeight() const;

    private:
        const int x, y, w, h;
    };
}
