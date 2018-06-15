#pragma once

#include <repositories/EmotionRepository.pb.h>

#include <goliath/foundation/repositories/repository.h>

/**
 * @file emotion_repository.h
 * @author Group 7 - Informatica
 */

namespace goliath::repositories {
    /**
     * @class goliath::repositories::EmotionRepository
     * @brief Repository for current emotion
     */
    class EmotionRepository : public Repository {
    public:
        using Emotion = proto::repositories::EmotionRepository::Emotion;

        EmotionRepository();

        std::unique_ptr<google::protobuf::Message> getMessage() override;

        const Emotion &getCurrentEmotion() const;
        void setCurrentEmotion(const Emotion &emotion);

    private:
        Emotion currentEmotion;
    };
}
