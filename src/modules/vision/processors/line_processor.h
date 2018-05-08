#pragma once

#include <opencv2/imgproc.hpp>

#include "processor.h"

namespace goliath::vision {
    class line_processor : public processor {
    public:
        explicit line_processor(const cv::Mat &input);

        // Detector chaining
        explicit line_processor(processor &other);

        cv::Mat process() const override;
    };
}