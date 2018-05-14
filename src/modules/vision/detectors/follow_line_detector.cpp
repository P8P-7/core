#include "follow_line_detector.h"

using namespace goliath::vision;
using namespace goliath::exceptions;

follow_line_detector::follow_line_detector(const cv::Mat& input, int boxes, int box_height, int boxes_bottom_margin,
                                           int boxes_horizontal_margin)
        : detector(input), boxes(boxes), box_height(box_height), boxes_bottom_margin(boxes_bottom_margin),
          boxes_horizontal_margin(boxes_horizontal_margin) {
    if(boxes > 4 || boxes < 0) {
        throw vision_error("The max amount of ROI boxes must be between 1 and 4");
    }
}

follow_line_detector::follow_line_detector(const follow_line_detector& other)
        : detector(other.input), boxes(other.boxes), box_height(other.box_height),
          boxes_bottom_margin(other.boxes_bottom_margin), boxes_horizontal_margin(other.boxes_horizontal_margin) {
}

std::vector<cv::Vec4d> follow_line_detector::detect() const {
    cv::Mat test = input;

    const int image_center_x = (input.cols - 1) / 2;
    const int image_center_y = input.rows - 1;

    int total = 0;
    int output_boxes = 0;

    for(int box_index = 0; box_index < boxes; ++box_index) {
        const int x = boxes_horizontal_margin;
        const int y = boxes_bottom_margin + box_height * (box_index + 1);
        const int w = (input.cols - 1) - (boxes_horizontal_margin * 2);
        const int h = box_height;
        roi_processor roi_processor(input, x, y, w, h);
        cv::Mat box = roi_processor.process();

        const int box_center_x = (box.cols - 1) / 2;

        cv::cvtColor(box, box, CV_BGR2GRAY);
        cv::GaussianBlur(box, box, cv::Size(9, 9), 2, 2);
        cv::threshold(box, box, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(box, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        if(contours.size() > 1 || contours.empty()) {
            continue;
        }

        std::vector<cv::Point> contour = contours[0];

        cv::Moments moments = cv::moments(contour, false);

        int center_x = static_cast<int>(moments.m10 / moments.m00);

        total += box_center_x - center_x;
        output_boxes++;
    }

    if(output_boxes == 0) {
        return {{ static_cast<double>(follow_line_direction::ERROR) }};
    }

    double difference = total / output_boxes;

    if(difference == 0) {
        return {{ static_cast<double>(follow_line_direction::ON_COURSE) }};
    }
    else if(difference > 0) {
        return {{static_cast<double>(follow_line_direction::LEFT)}};
    }
    else {
        return {{static_cast<double>(follow_line_direction::RIGHT)}};
    }
}
