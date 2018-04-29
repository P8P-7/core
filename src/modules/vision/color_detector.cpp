#include <goliath/color_detector.h>

using namespace goliath::vision;

cv::Mat color_detector::detect() const {
    cv::Mat threshold;
    cv::inRange(input, min, max, threshold);
    return threshold;
}