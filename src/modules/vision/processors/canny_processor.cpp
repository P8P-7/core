#include "canny_processor.h"

#include <opencv2/imgproc.hpp>

using namespace goliath::vision;

CannyProcessor::CannyProcessor(const cv::Mat& input, int minLowThreshold, int maxLowThreshold)
        : Processor(input), minLowThreshold(minLowThreshold), maxLowThreshold(maxLowThreshold) {
}

CannyProcessor::CannyProcessor(Processor& other, int minLowThreshold, int maxLowThreshold)
        : Processor(other.process()), minLowThreshold(minLowThreshold), maxLowThreshold(maxLowThreshold) {
}

cv::Mat CannyProcessor::process() const {
    cv::Mat destination;

    cv::Canny(input, destination, minLowThreshold, maxLowThreshold, DEFAULT_CANNY_KERNEL);

    return destination;
}
