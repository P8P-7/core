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

        void set_caller(const unsigned &command_id);
        const unsigned get_caller() const;

        bool operator()() const;

    private:
        bool locked = false;

        mutable unsigned caller = 0;
        std::condition_variable *var;
    };
}
