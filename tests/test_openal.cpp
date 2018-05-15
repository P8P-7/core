#define BOOST_TEST_MODULE test_openal

#include <boost/test/included/unit_test.hpp>
#include <goliath/openal.h>

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)
    BOOST_AUTO_TEST_CASE(test_microphone) {
        audio::microphone microphone(8000, AL_FORMAT_MONO16, 800);

        ALubyte buffer[200000];
        microphone.record(buffer, std::chrono::microseconds(5000000), std::chrono::microseconds(10000));

        std::cout << "Done" << std::endl;
    }
BOOST_AUTO_TEST_SUITE_END()
