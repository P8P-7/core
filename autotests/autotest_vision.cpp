#define BOOST_TEST_MODULE autotest_vision

#include <boost/test/included/unit_test.hpp>
#include <boost/format.hpp>
#include <goliath/vision.h>

using namespace goliath;

class TestVisionFixture {
public:
    TestVisionFixture() : imagePath(GOLIATH_TEST_IMAGE_DIR) {}
    std::string imagePath;
};

BOOST_FIXTURE_TEST_SUITE(BOOST_TEST_MODULE, TestVisionFixture)
    BOOST_AUTO_TEST_CASE(test_line_detection) {
        BOOST_TEST_CHECKPOINT("Loading image");
        cv::Mat input = cv::imread(imagePath + "highway.jpg", CV_BGR2GRAY);

        BOOST_TEST_CHECKPOINT("Creating a canny");
        vision::CannyProcessor cannyProcessor(input, 200, 50);
        cv::Mat cannyImage = cannyProcessor.process();

        BOOST_TEST_CHECKPOINT("Detecting lines");
        vision::LineDetector lineDetector(cannyImage, 125, 50, 2);
        std::vector<cv::Vec4d> lines = lineDetector.detect();

        BOOST_ASSERT(!lines.empty());
    }

    BOOST_AUTO_TEST_CASE(test_color_detection) {
        BOOST_TEST_CHECKPOINT("Loading image");
        cv::Mat input = cv::imread(imagePath + "mondriaan.jpg");
        cv::cvtColor(input, input, CV_BGR2HSV);

        BOOST_TEST_CHECKPOINT("Processing image too only keep reds");
        vision::ColorProcessor colorProcessor(input, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255));
        cv::Mat red = colorProcessor.process();

        BOOST_TEST_CHECKPOINT("Check if there is any red in image");
        for(int row = 0; row < red.rows; ++row) {
            for(int col = 0; col < red.cols; ++col) {
                cv::Vec3d pixel = red.at<cv::Vec3d>(row, col);
                if(cv::sum(pixel)[0] > 0) {
                    BOOST_ASSERT(true);
                    return;
                }
            }
        }

        BOOST_ASSERT(false);
    }

    BOOST_AUTO_TEST_CASE(test_roi) {
        BOOST_TEST_CHECKPOINT("Loading image");
        cv::Mat input = cv::imread(imagePath + "mondriaan.jpg");

        BOOST_TEST_CHECKPOINT("Create a ROI");
        vision::RoiProcessor roi(input, 10, 10, 200, 200);

        cv::Mat output = roi.process();
        BOOST_CHECK_EQUAL(output.cols, 200);
        BOOST_CHECK_EQUAL(output.rows, 200);
    }

    BOOST_AUTO_TEST_CASE(test_line_following) {
        BOOST_TEST_CHECKPOINT("Loading image");
        cv::Mat input = cv::imread(imagePath + "line.jpg");

        vision::FollowLineDetector followLineDetector(input, 4, 100, 100, 50, 10, 10000);
        std::vector<cv::Vec4d> output = followLineDetector.detect();

        BOOST_CHECK_EQUAL(output[0][0], vision::FollowLineDirection::RIGHT);
    }

BOOST_AUTO_TEST_SUITE_END()
