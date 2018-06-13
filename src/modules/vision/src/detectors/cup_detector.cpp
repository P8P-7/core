#include <goliath/vision/detectors/cup_detector.h>
#include <goliath/vision/processors/color_processor.h>

using namespace goliath::vision;

CupDetector::CupDetector(const cv::Mat &input, double widthScalar, double heightScalar, double widthPow,
                         double heightPow)
    : Detector(input), widthScalar(widthScalar), heightScalar(heightScalar), widthPow(widthPow), heightPow(heightPow) {}

std::vector<cv::Vec4d> CupDetector::detect() const {
    cv::Mat processed;

    cv::GaussianBlur(input, processed, cv::Size(5, 5), 0, 0);
    cv::threshold(processed, processed, 112, 255, cv::THRESH_BINARY);

    ColorProcessor colorProcessor(processed, COLOR_MIN, COLOR_MAX);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(colorProcessor.process(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    bool foundCup = false;

    std::vector<cv::Point> maxContour;
    double maxSize = MIN_CONTOUR_SIZE;

    for (const auto &contour : contours) {
        if (cv::contourArea(contour) > MIN_CONTOUR_SIZE && cv::contourArea(contour) > maxSize) {
            maxContour = contour;
            maxSize = cv::contourArea(contour);
            foundCup = true;
        }
    }

    if (!foundCup) {
        return {{0, 0, 0, 0}};
    }

    cv::Rect boundingRect = cv::boundingRect(maxContour);

    double heightDistance = heightScalar * pow(boundingRect.width, heightPow);
    double widthDistance = widthScalar * pow(boundingRect.height, widthPow);

    return {{1, heightDistance, widthDistance, 0}};
}
