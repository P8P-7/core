#include <goliath/controller/watcher.h>

#include <MessageCarrier.pb.h>
#include <boost/log/trivial.hpp>

using namespace goliath::repositories;
using namespace goliath;

void Watcher::synchronize() {
    proto::MessageCarrier carrier;
    proto::SynchronizeMessage *message = new proto::SynchronizeMessage;
    for (auto repo : repositories) {
        if (!repo->isInvalidated()) {
            continue;
        }

        auto repoPackage = repo->getMessage();
        message->add_messages()->PackFrom(*repoPackage);

        repo->validate();
    }
    carrier.set_allocated_synchronizemessage(message);

    publisher.publish(carrier);
}

void Watcher::invalidateAll() {
    for (auto repo : repositories) {
        repo->invalidate();
    }
}

bool Watcher::shouldSynchronize() const {
    for (auto repo : repositories) {
        if (repo->isInvalidated()) {
            return true;
        }
    }

    return false;
}

void Watcher::watch(std::shared_ptr<Repository> repo) {
    repositories.emplace_back(repo);
}

std::vector<std::shared_ptr<Repository>> Watcher::getRepositories() {
    return repositories;
}

Watcher::Watcher(int polling_rate, foundation::PublisherService &publisher)
    : pollingRate(polling_rate), publisher(publisher), running(false) {
}

Watcher::~Watcher() {
    if (running) {
        stop();
    }
}

void Watcher::start() {
    if (running) {
        throw std::runtime_error("Watcher has already been started");
    }

    running = true;
    invalidateAll();
    thread = std::thread(&Watcher::run, this);
}

void Watcher::stop() {
    if (!running) {
        throw std::runtime_error("Watcher isn't running");
    }

    running = false;
    thread.join();
}

void Watcher::run() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(pollingRate));

        if (shouldSynchronize()) {
            BOOST_LOG_TRIVIAL(debug) << "Broadcasting synchronize message";
            synchronize();
        }
    }
}
