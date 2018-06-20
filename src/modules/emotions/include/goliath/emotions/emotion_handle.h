#pragma once

#include <goliath/foundation.h>
#include "repositories/emotion_repository.h"

namespace goliath::handles {
    class EmotionHandle : public Handle {
    public:
        EmotionHandle(const size_t &handleId, const std::shared_ptr<repositories::EmotionRepository> &emotionRepository);

        std::shared_ptr<repositories::EmotionRepository> getEmotionRepository();

    private:
        std::shared_ptr<repositories::EmotionRepository> emotionRepository;
    };
}
