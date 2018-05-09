#include "within_box_detector.h"

using namespace goliath::vision;

within_box_detector::within_box_detector(const cv::Mat& input, int threshold, double min_line_length, double max_line_gap)
        : line_detector(input, threshold, min_line_length, max_line_gap) {
}

std::vector<cv::Vec4d> within_box_detector::detect() const {
    std::vector<cv::Vec4d> lines = line_detector::detect();
    std::vector<cv::Vec4d> edge_lines;

    for(cv::Vec4d line : lines) {
        if(point_at_edge(line[0], input.rows) || point_at_edge(line[1], input.cols) ||
                point_at_edge(line[2], input.rows) || point_at_edge(line[3], input.cols)) {
            edge_lines.push_back(line);
        }
    }

    return edge_lines;
}

bool within_box_detector::point_at_edge(int point, int edge) const {
    return point <= DISTANCE_FROM_EDGE_THRESHOLD || point > edge + DISTANCE_FROM_EDGE_THRESHOLD;
}

bool within_box_detector::line_is_corner(cv::Vec4d &line, std::vector<cv::Vec4d> &others) {
    for(cv::Vec4d other_line : others) {
        if(point_is_corner(line, other_line)) {
            return true;
        }
    }

    return false;
}

bool within_box_detector::point_is_corner(cv::Vec4d &line1, cv::Vec4d &line2) {
    for(int i = 0; i < 4; i += 2) {
        for(int j = 0; j < 4; j += 2) {
            cv::Vec4d line(i, i + 1, j, j+1);
            if(cv::norm(line) <= DISTANCE_FOR_CORNER_THRESHOLD) {
                return true;
            }
        }
    }
    return false;
}
