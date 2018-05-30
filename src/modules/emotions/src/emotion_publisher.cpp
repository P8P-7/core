#include <goliath/emotions/emotion_publisher.h>

using namespace goliath::emotions;

EmotionPublisher::EmotionPublisher(zmq::context_t& context, const std::string& host, int port)
        : ZmqPublisher(context, host, port), current(Emotion::NEUTRAL) {
}

EmotionPublisher::EmotionPublisher(const EmotionPublisher& other)
        : ZmqPublisher(other.context, other.host, other.port), current(other.current) {
}

bool EmotionPublisher::publishEmotion(const Emotion& emotion) {
    current = emotion;

    auto *emotionMessage = new EmotionMessage;
    emotionMessage->set_emotion(emotion);

    MessageCarrier carrier;
    carrier.set_allocated_emotionmessage(emotionMessage);

    return publish(carrier);
}

Emotion EmotionPublisher::getCurrentEmotion() const {
    return current;
}
