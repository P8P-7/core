#pragma once

#include <memory>

#include "reel.h"

namespace goliath::transitions {
    class ParallelReel {
    public:
        ParallelReel(std::vector<std::shared_ptr<Reel>> reels);

    private:
        std::vector<Reel> reels;
    };
}
