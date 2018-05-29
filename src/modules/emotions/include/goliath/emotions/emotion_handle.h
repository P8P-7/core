#pragma once

#include <goliath/foundation.h>

#include "emotion_publisher.h"

/**
 * @file emotion_handle.h
 * @author Group 7 - Informatica
 */

namespace goliath::handles {
    class EmotionHandle : public Handle {
    public:
        EmotionHandle(const size_t &id, emotions::EmotionPublisher &publisher);

        std::shared_ptr<emotions::EmotionPublisher> getPublisher();

    private:
        emotions::EmotionPublisher publisher;

    };
}
