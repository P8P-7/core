#pragma once

#include <iostream>
#include <string>
#include <zmq.hpp>

#include <MessageCarrier.pb.h>

const std::string PROTOCOL = "tcp";

namespace goliath::messaging {
    class zmq_io {
    public:
        zmq_io(zmq::context_t &context, const std::string &host, const int port)
                : zmq_io(context, host, port, 0) {}

        zmq_io(zmq::context_t &context, const std::string &host, const int port, const int socket_type)
                : context(context), host(host), port(port), socket(context, socket_type) {}

        std::string address() const;

        void connect();

        void bind();

    protected:
        zmq::context_t& context;
        const std::string host;
        const int port;
        zmq::socket_t socket;
    };
}
