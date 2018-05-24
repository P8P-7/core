#define BOOST_TEST_MODULE autotest_controller

#include <boost/test/included/unit_test.hpp>
#include <goliath/controller.h>

using namespace goliath;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)
    BOOST_AUTO_TEST_CASE(test_handle_lock) {
        handles::HandleMap handleMap;
        handleMap.add<handles::Handle>(0);

        handleMap[0]->lock(0);
        BOOST_CHECK_EQUAL(handleMap[0]->isLocked(), true);

        handleMap[0]->unlock();
        BOOST_CHECK_EQUAL(handleMap[0]->isLocked(), false);
    }
    
BOOST_AUTO_TEST_SUITE_END()
