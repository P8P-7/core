#include <goliath/vision/detectors/color_region_detector.h>
#include <goliath/vision/util/vision_error.h>

using namespace goliath::vision;
using namespace goliath::exceptions;

ColorRegionDetector::ColorRegionDetector(const cv::Mat& input, int hMin, int hMax, int sMin, int sMax)
        : Detector(input), hMin(hMin), hMax(hMax), sMin(sMin), sMax(sMax) {
}

std::vector<cv::Vec4d> ColorRegionDetector::detect() const {
    cv::Mat hsv;
    cv::cvtColor(input, hsv, CV_BGR2HSV);

    cv::MatND hist;

    float hRanges[] = {0, 360};
    float sRanges[] = {0, 100};
    int histSize[] = {static_cast<int>(hRanges[1]), static_cast<int>(sRanges[1])};
    const float* ranges[] = {hRanges, sRanges};
    const std::vector<int> channels{0, 1};

    cv::calcHist(&hsv, 1, &channels[0], cv::Mat(), hist, 2, histSize, ranges, true, false);

    cv::Point sh;
    cv::minMaxLoc(hist, nullptr, nullptr, nullptr, &sh);


    if(sh.x == 0 && sh.y == 0) {
        return {{-1, 0, 0, 0}};
    } else if(hMin > hMax && (sh.y >= hMin || sh.y <= hMax)){
        return {{1, 0, 0, 0}};
    } else if(sh.x >= sMin && sh.x <= sMax && sh.y >= hMin && sh.y <= hMax) {
        return {{1, 0, 0, 0}};
    } else {
        return {{0, 0, 0, 0}};
    }
}
