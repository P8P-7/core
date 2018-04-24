#pragma once

namespace goliath::io {
    class zmq_io {
    public:
        zmq_io(const std::string &host, const int port)
                : host(host), port(port), socket(context, 0) {}

        zmq_io(const std::string &address, const int port, const int socket_type)
                : host(host), port(port), socket(context, socket_type) {}

        std::string address() const;

        void connect(const std::string &address);
        void bind(const std::string &address);

    protected:
        const std::string host;
        const int port;
        zmq::context_t context = zmq::context_t(1);
        zmq::socket_t socket;
    };
}