#pragma once

#include "line_detector.h"

/**
 * @file within_box_detector.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    const int DISTANCE_FROM_EDGE_THRESHOLD = 5;
    const int DISTANCE_FOR_CORNER_THRESHOLD = 5;

    class WithinBoxDetector : public LineDetector {
    public:
        WithinBoxDetector(const cv::Mat &input, int threshold, double minLineLength, double maxLineGap);

        std::vector<cv::Vec4d> detect() const override;

    private:
        bool pointAtEdge(double point, int edge) const;
        bool lineIsCorner(cv::Vec4d& line, std::vector<cv::Vec4d>& others);
        bool pointIsCorner(cv::Vec4d& line1, cv::Vec4d& line2);
    };
}
