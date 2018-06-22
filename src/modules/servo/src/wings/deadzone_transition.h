#pragma once

#include <goliath/servo.h>

namespace goliath::servo {
    class DeadzoneTransitionController {
    public:
        DeadzoneTransitionController(
                WingController *controller,
                handles::WingHandle &handle,
                Direction direction,
                std::uint16_t speed);
        DeadzoneTransitionController(
                WingController *controller,
                servo::WingCommand command);

        void prestart();
        bool canStart();
        void start();
        bool tick();
        void stop();
        bool isInDeadzone() const;
        bool isStarted() const;
    private:
        const static int threshold = 256;
        const static std::size_t historySize = 1;
        const static std::size_t bufferSize = historySize * 4;
        const static int margin = 25;

        std::deque<int> history;
        bool started;

        WingController *controller;
        handles::WingHandle &handle;
        Direction direction;
        std::uint16_t speed;

        const servo::WingState &state;

        int getTargetPosition() const;
        int getThreshold() const;
        bool isInThreshold(int value) const;
        int getBoundaryPosition() const;
    };
}