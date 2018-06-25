#define BOOST_TEST_MODULE test_webcam

#include <boost/test/included/unit_test.hpp>
#include <goliath/vision.h>

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)
    BOOST_AUTO_TEST_CASE(test_webcam_png) {
        vision::Webcam camera(0);

        cv::Mat input = camera.getFrame();

        cv::imwrite("./output/test_webcam.jpg",input);
    }

BOOST_AUTO_TEST_SUITE_END()
