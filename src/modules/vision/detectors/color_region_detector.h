#pragma once

#include "detector.h"
#include "../util/vision_error.h"

/**
 * @file color_region_detector.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @class goliath::vision::ColorRegionDetector
     * @brief
     */
    class ColorRegionDetector : public Detector {
    public:
        ColorRegionDetector(const cv::Mat& input, int hMin, int hMax, int sMin, int sMax);

        std::vector<cv::Vec4d> detect() const override;

    private:
        int hMin, hMax, sMin, sMax;
    };
}
