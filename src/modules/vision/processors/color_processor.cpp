#include "color_processor.h"

using namespace goliath::vision;

ColorProcessor::ColorProcessor(const cv::Mat &inputBgr, const cv::Scalar &min, const cv::Scalar &max)
        : Processor(inputBgr), min(min), max(max) {
}

ColorProcessor::ColorProcessor(Processor& other, const cv::Scalar &min, const cv::Scalar &max)
        : Processor(other.process()), min(min), max(max) {
}

cv::Mat ColorProcessor::process() const {
    cv::Mat output;
    cv::inRange(input, min, max, output);
    return output;
}
