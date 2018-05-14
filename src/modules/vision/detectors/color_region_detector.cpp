#include "color_region_detector.h"

using namespace goliath::vision;

color_region_detector::color_region_detector(const cv::Mat& input)
    : detector(input) {
}
