#include <goliath/vision/processors/roi_processor.h>
#include <goliath/vision/util/vision_error.h>

using namespace goliath::vision;
using namespace goliath::exceptions;

RoiProcessor::RoiProcessor(const cv::Mat& input, int x, int y, int w, int h)
    : Processor(input), x(x), y(y), w(w), h(h) {
    if (x >= input.cols) {
        throw VisionError("X is outside image");
    }
    if (x + w >= input.cols) {
        throw VisionError("X + Width is outside image");
    }
    if (y >= input.rows) {
        throw VisionError("Y is outside image");
    }
    if (y + h >= input.rows) {
        throw VisionError("Y + Height is outside image");
    }
}

RoiProcessor::RoiProcessor(Processor& other, int x, int y, int w, int h)
    : Processor(other.process()), x(x), y(y), w(w), h(h) {
}

cv::Mat RoiProcessor::process() const {
    return cv::Mat(input, cv::Rect(x, y, w, h));
}
