#pragma once

#include "handles.h"

namespace goliath::handles {
    class handle_mutex {
    public:
        handle_mutex()
            : var(nullptr) {}

        handle_mutex(std::condition_variable *var)
            : var(var) {}

        void lock();
        void unlock();

        bool get();

    private:
        std::condition_variable *var;
        bool locked = false;
    };
}
