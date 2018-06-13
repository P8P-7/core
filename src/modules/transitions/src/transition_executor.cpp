#include <goliath/transitions/transition_executor.h>
#include <boost/log/trivial.hpp>

#include <chrono>
#include <thread>
#include <iostream>
#include <cmath>

using namespace goliath::transitions;

size_t TransitionExecutor::execute(std::shared_ptr<Tickable> tickable) {
    Microseconds interval = Microseconds(static_cast<int>(1.0 / tickable->getTicksPerSecond() * 1000000.0));

    TimePoint start = Clock::now();
    TimePoint end = start + tickable->getDuration();
    TimePoint current = start;

    size_t skippedTicks = 0;

    size_t currentSkips = 0;
    TimePoint intervalStart;

    while (current < end) {

        if (currentSkips > 0) {
            skippedTicks++;

            BOOST_LOG_TRIVIAL(info) << "Skipped a tick";
            std::this_thread::sleep_for(intervalStart - Clock::now() + (interval * (currentSkips + 1)));
            currentSkips = 0;

            current = Clock::now();

            continue;
        }

        intervalStart = Clock::now();

        tickable->tick();

        auto timeAfter = intervalStart - Clock::now() + interval;

        if (timeAfter.count() < 0) {
            currentSkips = static_cast<size_t>(std::abs(timeAfter.count() / interval.count() / 1000) + 1);
        } else {
            std::this_thread::sleep_for(timeAfter);
        }

        current = Clock::now();
    }

    return skippedTicks;
}
