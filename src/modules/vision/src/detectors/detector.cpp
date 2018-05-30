#include <goliath/vision/detectors/detector.h>

using namespace goliath::vision;

Detector::Detector(const cv::Mat& input)
        : input(input) {
}

Detector::Detector(const Detector& other)
        : Detector(other.input) {
}

Detector::~Detector() {
    input.release();
}

void Detector::update(cv::Mat& input) {
    this->input = input;
}
