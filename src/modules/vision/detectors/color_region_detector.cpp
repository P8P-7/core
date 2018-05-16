#include "color_region_detector.h"

using namespace goliath::vision;
using namespace goliath::exceptions;

color_region_detector::color_region_detector(const cv::Mat& input, int h_bins, int s_bins, int h_min, int h_max,
                                             int s_min, int s_max)
        : detector(input), h_bins(h_bins), s_bins(s_bins), h_min(h_min), h_max(h_max), s_min(s_min), s_max(s_max) {
}

std::vector<cv::Vec4d> color_region_detector::detect() const {
    cv::Mat hsv;
    cv::cvtColor(input, hsv, CV_BGR2HSV);

    cv::MatND hist;

    int histSize[] = {h_bins, s_bins};
    float h_ranges[] = {0, 360};
    float s_ranges[] = {0, 100};
    const float* ranges[] = {h_ranges, s_ranges};
    const std::vector<int> channels{0, 1};

    cv::calcHist(&hsv, 1, &channels[0], cv::Mat(), hist, 2, histSize, ranges, true, false);

    cv::Point sh;
    cv::minMaxLoc(hist, 0, 0, 0, &sh);

    if(sh.x == 0 && sh.y == 0) {
        return {{-1, 0, 0, 0}};
    } else if(sh.x >= s_min && sh.x <= s_max && sh.y >= h_min && sh.y <= h_max) {
        return {{1, 0, 0, 0}};
    } else {
        return {{0, 0, 0, 0}};
    }
}
