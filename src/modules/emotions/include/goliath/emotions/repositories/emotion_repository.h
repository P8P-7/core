#pragma once

#include <EmotionMessage.pb.h>
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
        EmotionRepository();

        std::unique_ptr<::google::protobuf::Message> getMessage();

        const proto::Emotion &getCurrentEmotion() const;
        void setCurrentEmotion(const proto::Emotion &emotion);

    private:
        proto::Emotion currentEmotion;
    };
}
