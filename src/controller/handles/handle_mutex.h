#pragma once

#include <condition_variable>

#include "handles.h"

namespace goliath::handles {
    class handle_mutex {
    public:
        handle_mutex() {}

        handle_mutex(std::shared_ptr<std::condition_variable> var)
            : var(var) {}

        void lock();
        void unlock();

        void set_locker(const unsigned &command_id);
        const unsigned get_locker() const;
        bool get_status() const;

    private:
        bool locked = false;

        mutable unsigned locker_id = 0;
        std::weak_ptr<std::condition_variable> var;
    };
}
