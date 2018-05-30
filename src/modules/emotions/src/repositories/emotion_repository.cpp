#include <goliath/emotions/repositories/emotion_repository.h>

using namespace goliath;

repositories::EmotionRepository::EmotionRepository()
        : currentEmotion(Emotion::NEUTRAL) {
}

std::unique_ptr<::google::protobuf::Message> repositories::EmotionRepository::getMessage() {
    ::EmotionRepository emotionRepository;

    emotionRepository.set_emotion(currentEmotion);

    return std::make_unique<::EmotionRepository>(emotionRepository);
}

const Emotion &repositories::EmotionRepository::getCurrentEmotion() const {
    return currentEmotion;
}

void repositories::EmotionRepository::setCurrentEmotion(const Emotion& emotion) {
    currentEmotion = emotion;
    invalidate();
}
