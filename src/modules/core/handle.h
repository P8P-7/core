#pragma once

#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <boost/optional.hpp>

namespace goliath::handles {
    class handle {
    public:
        handle();
        handle(const handle &other);

        /**
         * Lock the handle.
         * @param command_id the owning command @headerfile handle.h
         */
        void lock(const size_t& command_id);

        /**
         * Release the handle.
         */
        void unlock();

        /**
         * Wait for the lock to become available and acquire the lock.
         * @param command_id the owning command @headerfile handle.h
         */
        void wait_and_lock(const size_t &command_id);

        /**
         * @return command id of owning command in @headerfile handle.h
         */
        const size_t get_owner_id() const;

        /**
         * Indicates whether or not this handle has been locked.
         * @return true if this handle has been locked
         */
        bool is_locked() const;
    private:
        bool locked = false;

        boost::optional<size_t> owner_id;
        std::mutex mutex;
        std::condition_variable var;
    };
}