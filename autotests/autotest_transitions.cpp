#define BOOST_TEST_MODULE autotest_transitions

#include <thread>

#include <boost/test/included/unit_test.hpp>
#include <goliath/transitions.h>

using namespace goliath::transitions;

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)
    BOOST_AUTO_TEST_CASE(test_phases) {
        Phase phase(std::chrono::milliseconds(2000), 60, 30, 40, &methods::linear);

        BOOST_CHECK_EQUAL(phase.getNumberOfTicks(), 120);
    }

    BOOST_AUTO_TEST_CASE(test_reels) {
        auto phase = std::make_shared<Phase>(std::chrono::milliseconds(60), 60, 30, 36, &methods::linear);

        Reel reel(60);
        reel.addPhase(phase);

        BOOST_CHECK_EQUAL(reel.getNumberOfTicks(), 4);

        reel.addPhase(phase);

        BOOST_CHECK_EQUAL(reel.getNumberOfTicks(), 8);

        reel.tick();
        reel.tick();
        reel.tick();
        BOOST_CHECK_EQUAL(reel.getNumberOfTicks(), 36);
    }

    BOOST_AUTO_TEST_CASE(test_parallel_reels) {
        auto phase1 = std::make_shared<Phase>(std::chrono::milliseconds(100), 10, 0, 100, &methods::linear);
        auto phase2 = std::make_shared<Phase>(std::chrono::milliseconds(100), 10, 100, 200, &methods::linear);
        auto phase3 = std::make_shared<Phase>(std::chrono::milliseconds(100), 10, 200, 300, &methods::linear);

        double reelValue1 = 0;

        auto reel1 = std::make_shared<Reel>(10, [&reelValue1](double value){
            reelValue1 = value;
        });

        reel1->addPhase(phase1);
        reel1->addPhase(phase2);
        reel1->addPhase(phase3);

        double reelValue2 = 0;

        auto reel2 = std::make_shared<Reel>(10, [&reelValue2](double value){
            reelValue2 = value;
        });

        reel2->addPhase(phase1);
        reel2->addPhase(phase2);
        reel2->addPhase(phase3);

        ParallelReel parallelReel({reel1, reel2});

        parallelReel.tick();
        BOOST_TEST_CHECK(reelValue1, 100);
        BOOST_TEST_CHECK(reelValue2, 100);

        parallelReel.tick();
        BOOST_TEST_CHECK(reelValue1, 200);
        BOOST_TEST_CHECK(reelValue2, 200);

        parallelReel.tick();
        BOOST_TEST_CHECK(reelValue1, 300);
        BOOST_TEST_CHECK(reelValue2, 300);

        parallelReel.tick();
        BOOST_TEST_CHECK(reelValue1, 300);
        BOOST_TEST_CHECK(reelValue2, 300);
    }

    BOOST_AUTO_TEST_CASE(test_transition_executor) {
        auto phase = std::make_shared<Phase>(std::chrono::milliseconds(1000), 10, 0, 100, &methods::linear);

        int tickCount = 0;

        auto reel = std::make_shared<Reel>(10, [&tickCount](double value){
            tickCount++;
        });

        reel->addPhase(phase);


        TransitionExecutor::execute(std::static_pointer_cast<Tickable>(reel));

        BOOST_TEST_CHECK(tickCount, 10);
    }

    BOOST_AUTO_TEST_CASE(test_transition_executor_skip) {
        auto phase = std::make_shared<Phase>(std::chrono::milliseconds(1000), 10, 0, 100, &methods::linear);

        auto reel = std::make_shared<Reel>(10, [](double value){
            std::this_thread::sleep_for(std::chrono::milliseconds(110));
        });

        reel->addPhase(phase);

        BOOST_TEST_CHECK(TransitionExecutor::execute(std::static_pointer_cast<Tickable>(reel)), 5);
    }

    BOOST_AUTO_TEST_CASE(test_transition_executor_skip_multiple) {
        auto phase = std::make_shared<Phase>(std::chrono::milliseconds(1000), 10, 0, 100, &methods::linear);

        auto reel = std::make_shared<Reel>(10, [](double value){
            std::this_thread::sleep_for(std::chrono::milliseconds(210));
        });

        reel->addPhase(phase);

        BOOST_TEST_CHECK(TransitionExecutor::execute(std::static_pointer_cast<Tickable>(reel)), 3);
    }

BOOST_AUTO_TEST_SUITE_END()
