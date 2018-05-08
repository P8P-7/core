#include "color_processor.h"

using namespace goliath::vision;

color_processor::color_processor(const cv::Mat& input_hsv, const cv::Scalar min, const cv::Scalar max)
        : processor(input_hsv), min(min), max(max) {
}

color_processor::color_processor(processor& other, const cv::Scalar min, const cv::Scalar max)
        : processor(other.process()), min(min), max(max) {
}

cv::Mat color_processor::process() const {
    cv::Mat threshold;
    cv::inRange(input, min, max, threshold);
    return threshold;
}
