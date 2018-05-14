#pragma once

#include <vector>
#include <SynchronizeMessage.pb.h>
#include <atomic>
#include <thread>
#include "repository.h"
#include "../publisher_service.h"

namespace goliath::repositories {
    /**
     * @brief watches multiple repositories for changes and sends them to the publisher service
     */
    class watcher {
    public:
        /**
         * @param polling_rate rate at which the watcher thread polls each repository
         * @param publisher service to which the watcher should publish a @see SynchronizeMessage
         */
        watcher(int polling_rate, core::interfaces::publisher_service &publisher);
        /**
         * Destructs the watcher and calls stop()
         */
        ~watcher();

        /**
         * Invalidates all repositories watched by this watcher
         */
        void invalidate_all();

        /**
         * @return true when any of the repositories is invalidated
         */
        bool should_synchronize() const;

        /**
         * Adds a repository to the watchlist
         * @param repo repository to be watched
         */
        void watch(std::shared_ptr<repository> repo);

        /**
         * @return all repositories watched by this watcher
         */
        std::vector<std::shared_ptr<repository>> get_repositories();

        /**
         * Start the watcher thread
         */
        void start();

        /**
         * Stops the watcher thread
         */
        void stop();
    private:
        int polling_rate;
        core::interfaces::publisher_service &publisher;
        std::vector<std::shared_ptr<repository>> repositories;

        std::atomic<bool> running;
        std::thread thread;

        void synchronize();
        void run();
    };
}