#include "follow_line_detector.h"

using namespace goliath::vision;
using namespace goliath::exceptions;

follow_line_detector::follow_line_detector(const cv::Mat& input, int boxes, int box_height, int boxes_bottom_margin,
                                           int boxes_horizontal_margin, double min_contour_area,
                                           double max_contour_area)
    : detector(input), boxes(boxes), box_height(box_height), boxes_bottom_margin(boxes_bottom_margin),
      boxes_horizontal_margin(boxes_horizontal_margin), min_contour_area(min_contour_area),
      max_contour_area(max_contour_area) {
    if(boxes > 4 || boxes < 0) {
        throw vision_error("The amount of ROI boxes must be between 1 and 4");
    }

    if(box_height * boxes + boxes_bottom_margin > (input.rows - 1)) {
        throw vision_error("Can't fit all boxes in image");
    }
}

follow_line_detector::follow_line_detector(const follow_line_detector& other)
    : follow_line_detector(other.input, other.boxes, other.box_height, other.boxes_bottom_margin,
                           other.boxes_horizontal_margin, other.min_contour_area, other.max_contour_area) {
}

std::vector<cv::Vec4d> follow_line_detector::detect() const {
    for (int box_index = 0; box_index < boxes; ++box_index) {
        const int x = boxes_horizontal_margin;
        const int y = (input.rows - 1 - boxes_bottom_margin) - (box_height * (box_index + 1));
        const int w = (input.cols - 1) - (boxes_horizontal_margin * 2);
        const int h = box_height;

        const int box_left_edge = boxes_horizontal_margin;
        const int box_right_edge = (input.cols - 1) - boxes_horizontal_margin;

        roi_processor roi_processor(input, x, y, w, h);
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

        if (cv::contourArea(contour) < min_contour_area || cv::contourArea(contour) > max_contour_area) {
            continue;
        }

        cv::Moments moments = cv::moments(contour, false);

        double contour_center_x = moments.m10 / moments.m00;
        double offset = (box_center_x - contour_center_x) * -1;\

        double norm_distance = cv::abs(vision_utilities::map(offset, static_cast<double>((box.cols - 1)) / 2, 1.0));

        cv::Vec4d rect{static_cast<double>(x), static_cast<double>(y), static_cast<double>(w), static_cast<double>(h)};

        if (offset == 0) {
            return {{follow_line_direction::ON_COURSE, offset, 0, 0}, rect};
        } else if (offset < 0) {
            return {{follow_line_direction::LEFT, norm_distance, static_cast<double>(box_index), 0}, rect};
        } else if (offset > 0) {
            return {{follow_line_direction::RIGHT, norm_distance, static_cast<double>(box_index), 0}, rect};
        }
    }

    return {{follow_line_direction::NO_LINE, 0, 0, 0}};
}
