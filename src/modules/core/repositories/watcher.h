#pragma once

#include <atomic>
#include <thread>
#include <vector>
#include <memory>

#include "repository.h"
#include "../publisher_service.h"

/**
 * @file watcher.h
 * @author Group 7 - Informatica
 */

namespace goliath::repositories {
    /**
     * @class goliath::repositories::Watcher
     * @brief watches multiple repositories for changes and sends them to the publisher service
     */
    class Watcher {
    public:
        /**
         * @param pollingRate rate at which the watcher thread polls each repository
         * @param publisher service to which the watcher should publish a @see SynchronizeMessage
         */
        Watcher(int pollingRate, core::interfaces::PublisherService &publisher);
        /**
         * @brief Destructs the watcher and calls stop()
         */
        ~Watcher();

        /**
         * @brief Invalidates all repositories watched by this watcher
         */
        void invalidateAll();

        /**
         * @return true when any of the repositories is invalidated
         */
        bool shouldSynchronize() const;

        /**
         * @brief Adds a repository to the watchlist
         * @param repo repository to be watched
         */
        void watch(std::shared_ptr<Repository> repo);

        /**
         * @return all repositories watched by this watcher
         */
        std::vector<std::shared_ptr<Repository>> getRepositories();

        /**
         * @brief Start the watcher thread
         */
        void start();

        /**
         * @brief Stops the watcher thread
         */
        void stop();
    private:
        int pollingRate;
        core::interfaces::PublisherService &publisher;
        std::vector<std::shared_ptr<Repository>> repositories;

        std::atomic<bool> running;
        std::thread thread;

        void synchronize();
        void run();
    };
}
