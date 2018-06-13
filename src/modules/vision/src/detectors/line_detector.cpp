#include <goliath/vision/detectors/line_detector.h>
#include <goliath/vision/processors/canny_processor.h>

#include <algorithm>
#include <memory>

using namespace goliath::vision;

LineDetector::LineDetector(const cv::Mat &input, int threshold, double rho, double theta, double minLineLength, double maxLineGap)
        : Detector(input), threshold(threshold), rho(rho), theta(theta), minLineLength(minLineLength), maxLineGap(maxLineGap) { }

LineDetector::LineDetector(const cv::Mat& input, int threshold, double minLineLength, double maxLineGap)
        : LineDetector(input, threshold, DEFAULT_LINE_DETECTOR_RHO, DEFAULT_LINE_DETECTOR_THETA, minLineLength, maxLineGap) { }

LineDetector::LineDetector(const LineDetector &other)
        : LineDetector(other.input, other.threshold, other.rho, other.theta, other.minLineLength, other.maxLineGap) { }

std::vector<cv::Vec4d> LineDetector::detect() const {
    std::vector<cv::Vec4d> lines;
    cv::HoughLinesP(input, lines, rho, theta, threshold, minLineLength, maxLineGap);

    return lines;
}

std::vector<cv::Vec4d> LineDetector::longestLines(std::shared_ptr<Detector> detector) const {
    std::vector<cv::Vec4d> lines = detector->detect();

    std::sort(lines.begin(), lines.end(), [](cv::Vec4d a, cv::Vec4d b){
        return cv::norm(a) > cv::norm(b);
    });

    return lines;
}

std::vector<cv::Vec4d> LineDetector::longestLines()  {
    return longestLines(std::make_shared<LineDetector>(*this));
}
