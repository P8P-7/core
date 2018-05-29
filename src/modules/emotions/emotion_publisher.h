#pragma once

#include <goliath/zmq_messaging.h>

#include <EmotionMessage.pb.h>

/**
 * @file emotion_publisher.h
 * @author Group 7 - Informatica
 */

namespace goliath::emotions {
    class EmotionPublisher : public messaging::ZmqPublisher {
    public:
        EmotionPublisher(zmq::context_t& context, const std::string& host, int port);
        EmotionPublisher(const EmotionPublisher &other);

        bool publishEmotion(const Emotion &emotion);

        Emotion getCurrentEmotion() const;

    private:
        Emotion current;
    };
}
