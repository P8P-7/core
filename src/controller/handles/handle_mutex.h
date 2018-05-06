#pragma once

#include <condition_variable>

#include "handles.h"

namespace goliath::handles {
    class handle_mutex {
    public:
        handle_mutex();
        handle_mutex(const handle_mutex &other);

        void lock();
        void unlock();
        void wait();

        void set_locker(const unsigned &command_id);
        const unsigned get_locker() const;
        bool is_locked() const;
        
    private:
        bool locked = false;

        mutable unsigned locker_id;
        std::mutex mutex;
        std::condition_variable var;
    };
}
