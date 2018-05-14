#pragma once

#include "detector.h"

namespace goliath::vision {
    class color_region_detector : public detector {
    public:
        color_region_detector(const cv::Mat &input);
    };
}
