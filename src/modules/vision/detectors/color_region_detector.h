#pragma once

#include "detector.h"
#include "../util/vision_error.h"

/**
 * @file color_region_detector.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    class color_region_detector : public detector {
    public:
        color_region_detector(const cv::Mat& input, int h_bins, int s_bins, int h_min, int h_max, int s_min, int s_max);

        std::vector<cv::Vec4d> detect() const override;

    private:
        int h_bins, s_bins, h_min, h_max, s_min, s_max;
    };
}
