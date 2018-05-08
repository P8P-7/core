#pragma once

#include "detector.h"
#include "../processors/line_processor.h"

namespace goliath::vision {
    class line_detector : public detector {
    public:
        line_detector(const cv::Mat &input, double rho, double theta, int threshold, double min_line_length, double max_line_gap);
        line_detector(const cv::Mat &input, int threshold, double min_line_length, double max_line_gap);

        std::vector<cv::Vec4i> detect() const override;

    private:
        int threshold;
        double rho, theta, min_line_length, max_line_gap;
    };
}