#include <thread>
#include <boost/log/trivial.hpp>
#include <MessageCarrier.pb.h>
#include "watcher.h"
#include "repository.h"
#include "../publisher_service.h"

using namespace goliath::repositories;

void watcher::synchronize() {
    MessageCarrier carrier;
    SynchronizeMessage *message = new SynchronizeMessage;
    for (auto repo : repositories) {
        if (!repo->is_invalidated()) {
            continue;
        }

        auto repo_package = repo->get_message();
        message->add_messages()->PackFrom(*repo_package);

        repo->validate();
    }
    carrier.set_allocated_synchronizemessage(message);

    publisher.publish(carrier);
}

void watcher::invalidate_all() {
    for (auto repo : repositories) {
        repo->invalidate();
    }
}

bool watcher::should_synchronize() const {
    for (auto repo : repositories) {
        if (repo->is_invalidated()) {
            return true;
        }
    }

    return false;
}

void watcher::watch(std::shared_ptr<repository> repo) {
    repositories.emplace_back(repo);
}

std::vector<std::shared_ptr<repository>> watcher::get_repositories() {
    return repositories;
}

watcher::watcher(int polling_rate, core::interfaces::publisher_service &publisher) : polling_rate(polling_rate),
                                                                                     publisher(publisher),
                                                                                     running(false) { }

watcher::~watcher() {
    if (running) {
        stop();
    }
}

void watcher::start() {
    if (running) {
        throw std::runtime_error("Watcher has already been started");
    }

    running = true;
    invalidate_all();
    thread = std::thread(&watcher::run, this);
}

void watcher::stop() {
    if (!running) {
        throw std::runtime_error("Watcher isn't running");
    }

    running = false;
    thread.join();
}

void watcher::run() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(polling_rate));

        if (should_synchronize()) {
            BOOST_LOG_TRIVIAL(debug) << "Broadcasting synchronize message";
            synchronize();
        }
    }
}

