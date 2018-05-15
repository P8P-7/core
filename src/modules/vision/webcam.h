#pragma once

#include <opencv2/opencv.hpp>
#include <boost/optional.hpp>

/**
 * @file webcam.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @class goliath::vision::webcam
     * @brief Wrapper class around VideoCapture
     */
    class webcam {
    public:
        /**
         * @param cap_id VideoCapture to be used
         */
        explicit webcam(const int cap_id);
        /**
         * @param cap_id VideoCapture to be used
         * @param color Convert incoming image to a specific format, must always start with CV_BGR2<color>
         */
        webcam(const int cap_id, const int color);
        ~webcam();

        /**
         * @brief Capture a frame and assign it to argument
         * @param frame Frame to be assigned with captured frame
         */
        void get_frame(cv::Mat& frame);
        /**
         * @return Captured frame
         */
        cv::Mat get_frame();

    private:
        const int cap_id;
        /**
         * @brief Unset if not converted to specific color, else set to a CV_BGR2<color>
         */
        boost::optional<const int> color;

        cv::VideoCapture cap;
    };
}
