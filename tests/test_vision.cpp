#define BOOST_TEST_MODULE test_vision

#include <boost/test/included/unit_test.hpp>
#include <boost/format.hpp>
#include <goliath/vision.h>

using namespace goliath;

class test_vision_fixture {
public:
    test_vision_fixture() : image_path(GOLIATH_TEST_IMAGE_DIR) {}
    std::string image_path;
};

BOOST_FIXTURE_TEST_SUITE(BOOST_TEST_MODULE, test_vision_fixture)
    BOOST_AUTO_TEST_CASE(test_line_detection) {
        BOOST_TEST_CHECKPOINT("Loading image");
        cv::Mat input = cv::imread(image_path + "highway.jpg", CV_BGR2GRAY);

        BOOST_TEST_CHECKPOINT("Creating a canny");
        vision::canny_processor canny_processor(input, 200, 50);
        cv::Mat canny_image = canny_processor.process();

        BOOST_TEST_CHECKPOINT("Detecting lines");
        vision::line_detector line_detector(canny_image, 125, 50, 2);
        std::vector<cv::Vec4d> lines = line_detector.detect();

        BOOST_ASSERT(!lines.empty());
    }

    BOOST_AUTO_TEST_CASE(test_color_detection) {
        BOOST_TEST_CHECKPOINT("Loading image");
        cv::Mat input = cv::imread(image_path + "mondriaan.jpg");
        cv::cvtColor(input, input, CV_BGR2HSV);

        BOOST_TEST_CHECKPOINT("Processing image too only keep reds");
        vision::color_processor color_processor(input, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255));
        cv::Mat red = color_processor.process();

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
        cv::Mat input = cv::imread(image_path + "mondriaan.jpg");

        BOOST_TEST_CHECKPOINT("Create a ROI");
        vision::roi_processor roi(input, 10, 10, 200, 200);

        cv::Mat output = roi.process();
        BOOST_CHECK_EQUAL(output.cols, 200);
        BOOST_CHECK_EQUAL(output.rows, 200);
    }

    BOOST_AUTO_TEST_CASE(test_line_following) {
        BOOST_TEST_CHECKPOINT("Loading image");
        cv::Mat input = cv::imread(image_path + "line.jpg");

        vision::follow_line_detector follow_line_detector(input, 4, 100, 100, 50);
        follow_line_detector.detect();

        cv::waitKey();
    }
BOOST_AUTO_TEST_SUITE_END()
