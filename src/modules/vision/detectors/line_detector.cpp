#include "line_detector.h"

using namespace goliath::vision;

line_detector::line_detector(const cv::Mat &input, double rho, double theta, int threshold, double min_line_length, double max_line_gap)
        : detector(input), rho(rho), theta(theta), threshold(threshold), min_line_length(min_line_length), max_line_gap(max_line_gap) {
}

line_detector::line_detector(const cv::Mat& input, int threshold, double min_line_length, double max_line_gap)
        : line_detector(input, DEFAULT_LINE_DETECTOR_RHO, DEFAULT_LINE_DETECTOR_THETA, threshold, min_line_length, max_line_gap) {
}

std::vector<cv::Vec4d> line_detector::detect() const {
    std::vector<cv::Vec4d> lines;
    cv::HoughLinesP(input, lines, rho, theta, threshold, min_line_length, max_line_gap);

    return lines;
}

std::vector<cv::Vec4d> line_detector::longest_lines() const {
    std::vector<cv::Vec4d> lines = detect();

    std::sort(lines.begin(), lines.end(), [](cv::Vec4d a, cv::Vec4d b){
        return cv::norm(a) > cv::norm(b);
    });

    return lines;
}
