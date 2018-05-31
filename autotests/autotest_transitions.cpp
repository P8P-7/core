#define BOOST_TEST_MODULE autotest_transitions

#include <boost/test/included/unit_test.hpp>
#include <goliath/transitions.h>

using namespace goliath::transitions;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)
    BOOST_AUTO_TEST_CASE(test_phases) {
        Phase phase(std::chrono::milliseconds(2000), 60, 30, 40, [](double x) -> double { return x; });

        BOOST_CHECK_EQUAL(phase.getTicks(), 120);
    }

    BOOST_AUTO_TEST_CASE(test_reels) {
        auto phase = std::make_shared<Phase>(std::chrono::milliseconds(60), 60, 30, 36, [](double x) -> double {
            return x; });

        Reel reel(60);
        reel.addPhase(phase);

        BOOST_CHECK_EQUAL(reel.getTicks(), 3);

        reel.addPhase(phase);

        BOOST_CHECK_EQUAL(reel.getTicks(), 6);

        reel.tick();
        reel.tick();
        reel.tick();
        BOOST_CHECK_EQUAL(reel.tick(), 32);

    }

BOOST_AUTO_TEST_SUITE_END()
