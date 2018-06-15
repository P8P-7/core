#include <goliath/emotions/repositories/emotion_repository.h>

using namespace goliath::repositories;
using namespace goliath;

EmotionRepository::EmotionRepository()
        : currentEmotion(Emotion::EmotionRepository_Emotion_NEUTRAL) {
}

std::unique_ptr<::google::protobuf::Message> repositories::EmotionRepository::getMessage() {
    proto::repositories::EmotionRepository emotionRepository;

    return std::make_unique<proto::repositories::EmotionRepository>(emotionRepository);
}

const EmotionRepository::Emotion &repositories::EmotionRepository::getCurrentEmotion() const {
    return currentEmotion;
}

void repositories::EmotionRepository::setCurrentEmotion(const EmotionRepository::Emotion& emotion) {
    currentEmotion = emotion;
    invalidate();
}
