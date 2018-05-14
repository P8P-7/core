#include "processor.h"

using namespace goliath::vision;

processor::processor(const cv::Mat& input)
        : input(input) {
}

processor::~processor() {
    input.release();
}
