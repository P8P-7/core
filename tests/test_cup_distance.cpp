#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define BOOST_TEST_MODULE test_vision

#include <boost/test/included/unit_test.hpp>
#include <boost/format.hpp>
#include <goliath/vision.h>

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)

    BOOST_AUTO_TEST_CASE(test_center_of_shape) {
        vision::Webcam camera(1);
        cv::namedWindow("webcam");
        cv::namedWindow("thresh");
        int frames = 0;
        double totalHeight = 0;
        double totalWidth = 0;
        while (true) {
            cv::Mat input = camera.getFrame();
            cv::Mat blurred;
            cv::Mat thresh;
            cv::Mat mask;
            cv::GaussianBlur(input, blurred, cv::Size(5, 5), 0, 0);
            cv::threshold(blurred, thresh, 112, 255, cv::THRESH_BINARY);
            cv::inRange(thresh, cv::Scalar(0, 254, 0), cv::Scalar(0, 255, 0), mask);

            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Point> approx;
            cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
            cv::Rect bounding;
            double maxSize = 500;
            std::vector<cv::Point> maxContour;
            bool foundCup = false;
            for (std::vector<cv::Point> contour : contours) {
                if (cv::contourArea(contour) > 500 && cv::contourArea(contour) > maxSize) {
                    maxContour = contour;
                    maxSize = cv::contourArea(contour);
                    foundCup = true;
                }

            }
            if (foundCup) {
                auto M = cv::moments(maxContour);
                int cX = static_cast<int>(M.m10 / M.m00);
                int cY = static_cast<int>(M.m01 / M.m00);

                bounding = boundingRect(maxContour);
                cv::Point pt1, pt2;
                pt1.x = bounding.x;
                pt1.y = bounding.y;

                pt2.x = bounding.x + bounding.width;
                pt2.y = bounding.y + bounding.height;

                cv::rectangle(input, pt1, pt2, cv::Scalar(255, 0, 125), -1);
                cv::circle(input, cv::Point(cX, cY), 5, cv::Scalar(255, 255, 255), -1);
                cv::drawContours(input, std::vector<std::vector<cv::Point>>{maxContour}, -1, cv::Scalar(0, 255, 125),
                                 2);

                // TODO better calibration
                std::string heightDistance = std::to_string(2925 * pow(bounding.width, -0.863));
                std::string widthDistance = std::to_string(3807 * pow(bounding.height, -0.946));

                cv::putText(input, heightDistance, pt1, cv::FONT_HERSHEY_DUPLEX,
                            1.0, cv::Scalar(0, 0, 0));
                cv::putText(input, widthDistance, pt2, cv::FONT_HERSHEY_DUPLEX,
                            1.0, cv::Scalar(0, 0, 0));


//                if (bounding.height > 10 && bounding.width > 10) {
//                    frames++;
//                    totalWidth+=bounding.width;
//                    totalHeight+=bounding.height;
//                    std::cout << (totalWidth/frames) << ":\t" << (totalHeight/frames) << std::endl;
//                }
            }

            cv::imshow("thresh", thresh);
            cv::imshow("webcam", input);
            cv::waitKey(1);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
#pragma clang diagnostic pop