#pragma once

#include "detector.h"

/**
 * @file color_region_detector.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    class color_region_detector : public detector {
    public:
        color_region_detector(const cv::Mat &input);
    };
}
