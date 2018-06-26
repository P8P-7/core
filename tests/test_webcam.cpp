#define BOOST_TEST_MODULE test_webcam

#include <boost/test/included/unit_test.hpp>
#include <goliath/vision.h>

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)
    BOOST_AUTO_TEST_CASE(test_webcam) {
        vision::Webcam camera(0);

        cv::Mat input = camera.getFrame();

        cv::imwrite("./test_output/test_webcam.jpg",input);
    }

    BOOST_AUTO_TEST_CASE(test_webcam_line_roi) {
        vision::Webcam camera(0);

        cv::Mat input = camera.getFrame();

        vision::RoiProcessor roiProcessor(input, 0, 240, 639, 239);

        input = roiProcessor.process();

        cv::imwrite("./test_output/test_webcam_line_roi.jpg",input);
    }

    BOOST_AUTO_TEST_CASE(test_webcam_area_roi) {
        vision::Webcam camera(0);

        cv::Mat input = camera.getFrame();

        vision::RoiProcessor roiProcessor(input, 220, 390, 200, 80);

        input = roiProcessor.process();

        cv::imwrite("./test_output/test_webcam_area_roi.jpg",input);
    }

BOOST_AUTO_TEST_SUITE_END()
