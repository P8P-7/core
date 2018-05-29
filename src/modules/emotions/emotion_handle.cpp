#include "emotion_handle.h"

using namespace goliath::handles;

EmotionHandle::EmotionHandle(const size_t& id, emotions::EmotionPublisher &publisher)
        : Handle(id), publisher(publisher) {
}

std::shared_ptr<goliath::emotions::EmotionPublisher> EmotionHandle::getPublisher() {
    return std::make_shared<emotions::EmotionPublisher>(publisher);
}
