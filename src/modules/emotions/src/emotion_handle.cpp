#include <goliath/emotions/emotion_handle.h>

using namespace goliath;

handles::EmotionHandle::EmotionHandle(const size_t &handleId,
                             const std::shared_ptr<goliath::repositories::EmotionRepository> &emotionRepository)
    : Handle(handleId), emotionRepository(emotionRepository) {}

std::shared_ptr<goliath::repositories::EmotionRepository> handles::EmotionHandle::getEmotionRepository() {
    return emotionRepository;
}

