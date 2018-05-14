#include "canny_processor.h"

using namespace goliath::vision;

canny_processor::canny_processor(const cv::Mat& input, int min_low_threshold, int max_low_threshold)
        : processor(input), min_low_threshold(min_low_threshold), max_low_threshold(max_low_threshold) {
}

canny_processor::canny_processor(processor& other, int min_low_threshold, int max_low_threshold)
        : processor(other.process()), min_low_threshold(min_low_threshold), max_low_threshold(max_low_threshold) {
}

cv::Mat canny_processor::process() const {
    cv::Mat destination;

    cv::Canny(input, destination, min_low_threshold, max_low_threshold, DEFAULT_CANNY_KERNEL);

    return destination;
}
