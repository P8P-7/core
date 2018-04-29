#pragma once

#include <opencv2/imgproc.hpp>

#include "detector.h"

namespace goliath::vision {
    class line_detector : public detector {
    public:
        line_detector(const cv::Mat input)
            : detector(input) {}

        // Detector chaining
        line_detector(detector &other)
            : detector(other.detect()) {}

        cv::Mat detect() const override;
    };
}