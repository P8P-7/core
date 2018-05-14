#include "roi_processor.h"

using namespace goliath::vision;
using namespace goliath::exceptions;

roi_processor::roi_processor(const cv::Mat& input, int x, int y, int w, int h)
    : processor(input), x(x), y(y), w(w), h(h) {
    if(x >= input.cols) {
        throw vision_error("X is outside image");
    }
    if(x + w >= input.cols) {
        throw vision_error("X + Width is outside image");
    }
    if(y >= input.rows) {
        throw vision_error("Y is outside image");
    }
    if(y + h >= input.rows) {
        throw vision_error("Y + Height is outside image");
    }
}

roi_processor::roi_processor(processor& other, int x, int y, int w, int h)
    : processor(other.process()), x(x), y(y), w(w), h(h){
}

cv::Mat roi_processor::process() const {
    return cv::Mat(input, cv::Rect(x, y, w, h));
}
