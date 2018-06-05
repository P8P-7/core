#include <goliath/emotions/repositories/emotion_repository.h>

using namespace goliath;

repositories::EmotionRepository::EmotionRepository()
        : currentEmotion(proto::Emotion::NEUTRAL) {
}

std::unique_ptr<::google::protobuf::Message> repositories::EmotionRepository::getMessage() {
    proto::repositories::EmotionRepository emotionRepository;

    emotionRepository.set_emotion(currentEmotion);

    return std::make_unique<proto::repositories::EmotionRepository>(emotionRepository);
}

const proto::Emotion &repositories::EmotionRepository::getCurrentEmotion() const {
    return currentEmotion;
}

void repositories::EmotionRepository::setCurrentEmotion(const proto::Emotion& emotion) {
    currentEmotion = emotion;
    invalidate();
}
