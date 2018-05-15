#pragma once

#include "detector.h"
#include "line_detector.h"

/**
 * @file within_box_detector.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    const int DISTANCE_FROM_EDGE_THRESHOLD = 5;
    const int DISTANCE_FOR_CORNER_THRESHOLD = 5;

    class within_box_detector : public line_detector {
    public:
        within_box_detector(const cv::Mat &input, int threshold, double min_line_length, double max_line_gap);

        std::vector<cv::Vec4d> detect() const override;

    private:
        bool point_at_edge(int point, int edge) const;
        bool line_is_corner(cv::Vec4d &line, std::vector<cv::Vec4d> &others);
        bool point_is_corner(cv::Vec4d &line1, cv::Vec4d &line2);
    };
}
