#pragma once

#include "detector.h"

/**
 * @file cup_detector.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    const cv::Scalar COLOR_MIN(0, 254, 0);
    const cv::Scalar COLOR_MAX(0, 255, 0);
    constexpr double MIN_CONTOUR_SIZE = 500;

    class CupDetector : public Detector {
    public:
        CupDetector(const cv::Mat &input, double widthScalar, double heightScalar, double widthPow, double heightPow);

        std::vector<cv::Vec4d> detect() const override;

    private:
        double widthScalar, heightScalar, widthPow, heightPow;

    };
}
