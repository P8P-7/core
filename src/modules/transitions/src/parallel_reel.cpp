#include <goliath/transitions/parallel_reel.h>
#include <goliath/transitions/util/transition_error.h>

using namespace goliath::transitions;

ParallelReel::ParallelReel(std::vector<std::shared_ptr<Reel>> reels) {
    if (reels.size() < 2) {
        throw goliath::exceptions::TransitionError("Parallel reel must contain at least 2 reels");
    }

    for (int i = 1; i < reels.size(); ++i) {
        if (reels[i]->getTicksPerSecond() != reels[i - 1]->getTicksPerSecond()) {
            throw goliath::exceptions::TransitionError("All reelsin a parallel reel must have the same tps");
        }
    }

    reels = reels;
}
