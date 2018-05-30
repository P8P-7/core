#pragma once

#include <goliath/zmq-messaging.h>
#include <EmotionMessage.pb.h>

#include "repositories/emotion_repository.h"

/**
 * @file emotion_publisher.h
 * @author Group 7 - Informatica
 */

namespace goliath::emotions {
    class EmotionPublisher : public messaging::ZmqPublisher {
    public:
        EmotionPublisher(zmq::context_t& context, const std::string& host, int port,
                         std::shared_ptr<repositories::EmotionRepository> repository);

        EmotionPublisher(const EmotionPublisher& other);

        bool publishEmotion(const Emotion& emotion);

    private:
        std::shared_ptr<repositories::EmotionRepository> repository;
    };
}
