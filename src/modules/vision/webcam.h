#pragma once

#include <opencv2/opencv.hpp>
#include <boost/optional.hpp>

/**
 * @file webcam.h
 * @author Group 7 - Informatica
 */

namespace goliath::vision {
    /**
     * @class goliath::vision::Webcam
     * @brief Wrapper class around VideoCapture
     */
    class Webcam {
    public:
        /**
         * @param capId VideoCapture to be used
         */
        explicit Webcam(const int capId);
        /**
         * @param capId VideoCapture to be used
         * @param color Convert incoming image to a specific format, must always start with CV_BGR2<color>
         */
        Webcam(const int capId, const int color);
        ~Webcam();

        /**
         * @brief Capture a frame and assign it to argument
         * @param frame Frame to be assigned with captured frame
         */
        void getFrame(cv::Mat& frame);
        /**
         * @return Captured frame
         */
        cv::Mat getFrame();

    private:
        const int capId;
        cv::VideoCapture cap;

        /**
         * @brief Unset if not converted to specific color, else set to a CV_BGR2<color>
         */
        boost::optional<const int> color;
    };
}
