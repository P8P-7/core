#include <goliath/line_detector.h>
#include <cmath>

using namespace goliath::vision;

cv::Mat line_detector::detect() const {
    cv::Mat destination;
    cv::Mat color_destination;

    cv::Canny(input, destination, 50, 200, 3);

    return destination;
}