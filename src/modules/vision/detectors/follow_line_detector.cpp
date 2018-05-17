#include "follow_line_detector.h"

#include <cmath>

#include "../util/vision_error.h"
#include "../util/vision_utilities.h"
#include "../processors/roi_processor.h"

using namespace goliath::vision;
using namespace goliath::exceptions;

FollowLineDetector::FollowLineDetector(const cv::Mat& input, int boxes, int boxHeight, int boxesBottomMargin,
                                       int boxesHorizontalMargin, double minContourArea, double maxContourArea)
    : Detector(input), boxes(boxes), boxHeight(boxHeight), boxesBottomMargin(boxesBottomMargin),
      boxesHorizontalMargin(boxesHorizontalMargin), minContourArea(minContourArea), maxContourArea(maxContourArea) {
    if (boxes > 4 || boxes < 0) {
        throw VisionError("The amount of ROI boxes must be between 1 and 4");
    }

    if (boxHeight * boxes + boxesBottomMargin > (input.rows - 1)) {
        throw VisionError("Can't fit all boxes in image");
    }
}

FollowLineDetector::FollowLineDetector(const FollowLineDetector& other)
    : FollowLineDetector(other.input, other.boxes, other.boxHeight, other.boxesBottomMargin,
                         other.boxesHorizontalMargin, other.minContourArea, other.maxContourArea) {
}

std::vector<cv::Vec4d> FollowLineDetector::detect() const {
    // Loop through all the boxes, from bottom up
    for (int box_index = 0; box_index < boxes; ++box_index) {
        const int x = boxesHorizontalMargin;
        const int y = (input.rows - 1 - boxesBottomMargin) - (boxHeight * (box_index + 1));
        const int w = (input.cols - 1) - (boxesHorizontalMargin * 2);
        const int h = boxHeight;

        // Create box roi
        RoiProcessor roi_processor(input, x, y, w, h);
        cv::Mat box = roi_processor.process();

        const int box_center_x = (box.cols - 1) / 2;

        cv::cvtColor(box, box, CV_BGR2GRAY);
        cv::GaussianBlur(box, box, cv::Size(9, 9), 2, 2);
        cv::threshold(box, box, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(box, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        if (contours.size() > 1 || contours.empty()) {
            continue;
        }

        std::vector<cv::Point> contour = contours[0];

        if (cv::contourArea(contour) < minContourArea || cv::contourArea(contour) > maxContourArea) {
            continue;
        }

        cv::Moments moments = cv::moments(contour, false);

        // Get contour center and compare it to the box center
        double contour_center_x = moments.m10 / moments.m00;
        double offset = (box_center_x - contour_center_x) * -1;\

        double norm_distance = cv::abs(VisionUtilities::map(offset, static_cast<double>((box.cols - 1)) / 2, 1.0));

        cv::Vec4d rect{static_cast<double>(x), static_cast<double>(y), static_cast<double>(w), static_cast<double>(h)};

        if (offset == 0) {
            return {{FollowLineDirection::ON_COURSE, offset, 0, 0}, rect};
        } else if (offset < 0) {
            return {{FollowLineDirection::LEFT, norm_distance, static_cast<double>(box_index), 0}, rect};
        } else if (offset > 0) {
            return {{FollowLineDirection::RIGHT, norm_distance, static_cast<double>(box_index), 0}, rect};
        }
    }

    return {{FollowLineDirection::NO_LINE, 0, 0, 0}};
}
