#pragma once

#include <thread>
#include <MessageCarrier.pb.h>

#include "zmq_io.h"

/**
 * @file zmq_subscriber.h
 * @author Group 7 - Informatica
 */

namespace goliath::messaging {
    /**
     * @class goliath::messaging::ZmqSubscriber
     * @brief Subscribes to host to receive message and process them in a callback
     */
    class ZmqSubscriber : public ZmqIo {
    public:
        using zmqMessageTopic = MessageCarrier::MessageCase;
        using zmqMessageCallback = std::function<void(const MessageCarrier&)>;

        /**
         * @param context @see goliath::messaging::ZmqIo
         * @param host @see goliath::messaging::ZmqIo
         * @param port @see goliath::messaging::ZmqIo
         */
        ZmqSubscriber(zmq::context_t &context, const std::string &host, const int port);
        ~ZmqSubscriber();

        /**
         * @brief Bind to host
         * @param topic Topic to bind on
         * @param callback Callback to be executed when an incoming message comes in
         */
        void bind(const zmqMessageTopic &topic, zmqMessageCallback callback);
        /**
         * @brief Start subscriber
         */
        void start();
        /**
         * @brief Stop subscriber
         */
        void stop();
    private:
        const std::string interrupterAddress = "inproc://interrupt";
        zmq::socket_t *interruptSocket;

        std::vector<zmq::pollitem_t> poll;
        std::map<zmqMessageTopic, zmqMessageCallback> callbacks;

        bool running = false;
        std::thread thread;

        void run();
    };
}
