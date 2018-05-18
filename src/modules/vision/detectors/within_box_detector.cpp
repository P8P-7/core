#include "within_box_detector.h"

using namespace goliath::vision;

WithinBoxDetector::WithinBoxDetector(const cv::Mat& input, int threshold, double minLineLength, double maxLineGap)
        : LineDetector(input, threshold, minLineLength, maxLineGap) {
}

std::vector<cv::Vec4d> WithinBoxDetector::detect() const {
    std::vector<cv::Vec4d> lines = LineDetector::detect();
    std::vector<cv::Vec4d> edgeLines;

    for(cv::Vec4d line : lines) {
        if(pointAtEdge(line[0], input.rows) || pointAtEdge(line[1], input.cols) ||
            pointAtEdge(line[2], input.rows) || pointAtEdge(line[3], input.cols)) {
            edgeLines.push_back(line);
        }
    }

    return edgeLines;
}

bool WithinBoxDetector::pointAtEdge(int point, int edge) const {
    return point <= DISTANCE_FROM_EDGE_THRESHOLD || point > edge + DISTANCE_FROM_EDGE_THRESHOLD;
}

bool WithinBoxDetector::lineIsCorner(cv::Vec4d &line, std::vector<cv::Vec4d> &others) {
    for(cv::Vec4d other_line : others) {
        if(pointIsCorner(line, other_line)) {
            return true;
        }
    }

    return false;
}

bool WithinBoxDetector::pointIsCorner(cv::Vec4d &line1, cv::Vec4d &line2) {
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
