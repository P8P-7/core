#include "detector.h"

using namespace goliath::vision;

detector::detector(const cv::Mat& input)
        : input(input) {
}

detector::detector(const detector& other)
        : detector(other.input) {
}

detector::~detector() {
    input.release();
}
