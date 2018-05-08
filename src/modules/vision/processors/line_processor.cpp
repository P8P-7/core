#include "line_processor.h"

using namespace goliath::vision;

line_processor::line_processor(const cv::Mat& input)
        : processor(input) {
}

line_processor::line_processor(processor& other)
        : processor(other.process()) {
}

cv::Mat line_processor::process() const {
    cv::Mat destination;

    cv::Canny(input, destination, 50, 200, 3);

    return destination;
}