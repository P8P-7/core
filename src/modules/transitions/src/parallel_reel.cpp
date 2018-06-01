#include <goliath/transitions/parallel_reel.h>
#include <goliath/transitions/util/transition_error.h>
#include <iostream>

using namespace goliath::transitions;

ParallelReel::ParallelReel(std::vector<std::shared_ptr<Reel>> reels)
        : Tickable(reels[0]->getTicksPerSecond()), reels(reels) {
    if (reels.size() < 2) {
        throw goliath::exceptions::TransitionError("Parallel reel must contain at least 2 reels");
    }

    for (size_t i = 1; i < reels.size(); ++i) {
        if (reels[i]->getTicksPerSecond() != reels[i - 1]->getTicksPerSecond()) {
            throw goliath::exceptions::TransitionError("All reels in a parallel reel must have the same tps");
        }
    }
}

void ParallelReel::tick() {
    for (auto &reel : reels) {
        if (reel->canTick()) {
            std::cout << "Tick";
            reel->tick();
        }
    }
}

std::chrono::milliseconds ParallelReel::getDuration() const {
    std::chrono::milliseconds longest = reels[0]->getDuration();

    for (size_t i = 1; i < reels.size(); ++i) {
        if (reels[i]->getDuration() > longest) {
            longest = reels[i]->getDuration();
        }
    }

    return longest;
}

const unsigned ParallelReel::getTicksPerSecond() const {
    return reels[0]->getTicksPerSecond();
}
