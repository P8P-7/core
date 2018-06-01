#include <goliath/transitions/transition_executor.h>
#include <boost/log/trivial.hpp>

#include <chrono>
#include <thread>
#include <iostream>

using namespace goliath::transitions;

void TransitionExecutor::execute(std::shared_ptr<Tickable> tickable) {
    Mis interval = Mis(static_cast<int>(1.0 / tickable->getTicksPerSecond() * 1000000.0));

    TimePoint start = Clock::now();
    TimePoint end = start + tickable->getDuration();
    TimePoint current = start;

    bool skip = false;

    while (current < end) {
        if (skip) {
            skip = false;
            BOOST_LOG_TRIVIAL(warning) << "Skipping tick";
            continue;
        }

        TimePoint intervalStart = Clock::now();

        tickable->tick();

        auto timeAfter = intervalStart - Clock::now() + interval;

        if (std::chrono::duration_cast<Ms>(timeAfter).count() < 0) {
            skip = true;
        }

        std::this_thread::sleep_for(timeAfter);

        current = Clock::now();
    }
}
