#define BOOST_TEST_MODULE test_vision

#include <boost/test/included/unit_test.hpp>
#include <boost/format.hpp>
#include <goliath/vision.h>

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)
    BOOST_AUTO_TEST_CASE(test_webcam_line_following) {
        vision::Webcam camera(0);
        cv::namedWindow("webcam");

        while (true) {
            cv::Mat input = camera.getFrame();
            vision::FollowLineDetector followLineDetector(input, 4, 100, 20, 0, 10, 10000);

            std::vector<cv::Vec4d> output = followLineDetector.detect();

            cv::rectangle(input, cv::Rect(output[1][0], output[1][1], output[1][2], output[1][3]),
                          cv::Scalar(0, 0, 255), 2);

            std::cout << "Direction: " << output[0][0]
                      << " Offset:    " << output[0][1] << std::endl;

            cv::imshow("webcam", input);
            cv::waitKey(1);
        }
    }

    BOOST_AUTO_TEST_CASE(test_webcam_colored_spot) {
        vision::Webcam camera(0, CV_BGR2HSV);
        cv::namedWindow("webcam");

        while (true) {
            cv::Mat input = camera.getFrame();
            vision::ColorRegionDetector colorRegionDetector(input, 150, 200, 50, 100);

            std::cout << colorRegionDetector.detect()[0][0] << std::endl;

            cv::imshow("webcam", input);
            cv::waitKey(1);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
