#pragma once

#include <string>
#include <zmq.hpp>

namespace goliath::io {
    class zmq_io {
    public:
        zmq_io(const char *address)
            : address(address), socket(context, 0) {}
        zmq_io(const char *address, const int socket_type)
            : address(address), socket(context, socket_type) {}

        void connect(const char* address);
        void bind(const char* address);

    protected:
        const char *address;
        zmq::context_t context = zmq::context_t(1);
        zmq::socket_t socket;
    };

    class zmq_publisher : public zmq_io {
    public:
        zmq_publisher(const char *address, const std::string &topic);

        bool publish(const std::string &message);

    private:
        const std::string topic;
    };

    class zmq_subscriber : public zmq_io {
    public:
        zmq_subscriber(const char *address, const std::string &topic);

        char* get();

    private:
        const std::string topic;
    };
}