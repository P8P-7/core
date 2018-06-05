#include <goliath/emotions/emotion_publisher.h>

using namespace goliath::emotions;

EmotionPublisher::EmotionPublisher(zmq::context_t& context, const std::string& host, int port,
                                   std::shared_ptr<repositories::EmotionRepository> repository)
    : ZmqPublisher(context, host, port), repository(repository) {
}

EmotionPublisher::EmotionPublisher(const EmotionPublisher& other)
    : ZmqPublisher(other.context, other.host, other.port), repository(other.repository) {
}

bool EmotionPublisher::publishEmotion(const proto::Emotion& emotion) {
    repository->setCurrentEmotion(emotion);

    auto* emotionMessage = new proto::EmotionMessage;
    emotionMessage->set_emotion(emotion);

    proto::MessageCarrier carrier;
    carrier.set_allocated_emotionmessage(emotionMessage);

    return publish(carrier);
}

