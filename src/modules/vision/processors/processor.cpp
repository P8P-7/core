#include "processor.h"

using namespace goliath::vision;

Processor::Processor(const cv::Mat& input)
        : input(input) {
}

Processor::~Processor() {
    input.release();
}
