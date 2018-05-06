#pragma once

#include <condition_variable>

#include "handles.h"

namespace goliath::handles {
    class handle_mutex {
    public:
        handle_mutex() {}

        void lock();
        void unlock();
        void wait();

        void set_locker(const unsigned &command_id);
        const unsigned get_locker() const;
        bool is_locked() const;
    private:
        bool locked = false;

        mutable unsigned locker_id = 0;
        std::mutex mutex;
        std::condition_variable var;
    };
}
