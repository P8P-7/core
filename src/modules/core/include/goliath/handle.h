#pragma once

#include <cstdlib>
#include <mutex>
#include <condition_variable>

namespace goliath::handles {
    class handle {
    public:
        handle();
        handle(const handle &other);

        void lock(const size_t& command_id);
        void unlock();
        void wait();

        const size_t get_owner_id() const;
        bool is_locked() const;
    private:
        bool locked = false;

        size_t owner_id;
        std::mutex mutex;
        std::condition_variable var;
    };
}