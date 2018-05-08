#include "line_detector.h"

using namespace goliath::vision;

line_detector::line_detector(const cv::Mat &input, double rho, double theta, int threshold, double min_line_length, double max_line_gap)
        : detector(input), rho(rho), theta(theta), threshold(threshold), min_line_length(min_line_length), max_line_gap(max_line_gap) {
}

line_detector::line_detector(const cv::Mat& input, int threshold, double min_line_length, double max_line_gap)
        : line_detector(input, 1, CV_PI / 180, threshold, min_line_length, max_line_gap) {
}

std::vector<cv::Vec4i> line_detector::detect() const {
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(input, lines, rho, theta, threshold, min_line_length, max_line_gap);

    return lines;
}
