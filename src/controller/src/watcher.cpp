#include <goliath/controller/watcher.h>

#include <MessageCarrier.pb.h>
#include <boost/log/trivial.hpp>

using namespace goliath::repositories;
using namespace goliath;

Watcher::Watcher(int polling_rate, foundation::PublisherService &publisher,
                 std::shared_ptr<commands::CommandExecutor> &executor)
        : pollingRate(polling_rate), publisher(publisher), running(false), commandExecutor(executor) {
}

Watcher::~Watcher() {
    if (running) {
        stop();
    }
}

void Watcher::synchronize() {
    proto::MessageCarrier carrier;
    proto::SynchronizeMessage *message = new proto::SynchronizeMessage;
    for (const auto &repo : repositories) {
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
    for (const auto &repo : repositories) {
        repo->invalidate();
    }
}

bool Watcher::shouldSynchronize() const {
    for (const auto &repo : repositories) {
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

        for (auto &repo : getRepositories()) {
            if (dynamic_cast<repositories::PollingRepository *>(repo.get())) {
                auto pollingRepository = std::dynamic_pointer_cast<repositories::PollingRepository>(repo);
                for (auto message : pollingRepository->getCommandMessages()) {
                    commandExecutor->run(pollingRepository->getPollingCommandId(), message);
                }
            }
        }
    }
}
