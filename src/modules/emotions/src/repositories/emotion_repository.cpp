#include <goliath/emotions/repositories/emotion_repository.h>

using namespace goliath::repositories;
using namespace goliath;

EmotionRepository::EmotionRepository()
        : currentEmotion(Emotion::ANGRY) {
}

std::unique_ptr<::google::protobuf::Message> repositories::EmotionRepository::getMessage() {
    proto::repositories::EmotionRepository emotionRepository;

    emotionRepository.set_emotion(currentEmotion);

    return std::make_unique<proto::repositories::EmotionRepository>(emotionRepository);
}

const EmotionRepository::Emotion::Emotion &repositories::EmotionRepository::getCurrentEmotion() const {
    return currentEmotion;
}

void repositories::EmotionRepository::setCurrentEmotion(const Emotion::Emotion& emotion) {
    currentEmotion = emotion;
    invalidate();
}
