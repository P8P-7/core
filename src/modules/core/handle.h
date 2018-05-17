#pragma once

#include <mutex>
#include <condition_variable>
#include <boost/optional.hpp>

/**
 * @file handle.h
 * @author Group 7 - Informatica
 */


namespace goliath::handles {
    class Handle {
    public:
        Handle();
        Handle(const Handle &other);

        /**
         * @brief Lock the handle.
         * @param commandId the owning command @headerfile handle.h
         */
        void lock(const size_t& commandId);

        /**
         * @brief Release the handle.
         */
        void unlock();

        /**
         * @brief Wait for the lock to become available and acquire the lock.
         * @param commandId the owning command @headerfile handle.h
         */
        void waitAndLock(const size_t &commandId);

        /**
         * @return command id of owning command in @headerfile handle.h
         */
        const size_t getOwnerId() const;

        /**
         * @brief Indicates whether or not this handle has been locked.
         * @return true if this handle has been locked
         */
        bool isLocked() const;
    private:
        bool locked = false;

        boost::optional<size_t> ownerId;
        std::mutex mutex;
        std::condition_variable var;
    };
}
