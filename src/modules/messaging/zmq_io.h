#pragma once

#include <string>
#include <zmq.hpp>

const std::string PROTOCOL = "tcp";

namespace goliath::messaging {
    class ZmqIo {
    public:
        ZmqIo(zmq::context_t &context, const std::string &host, const int port)
                : ZmqIo(context, host, port, 0) {}

        ZmqIo(zmq::context_t &context, const std::string &host, const int port, const int socketType)
                : context(context), host(host), port(port), socket(context, socketType) {}

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
