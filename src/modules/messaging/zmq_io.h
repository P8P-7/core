#pragma once

#include <string>
#include <zmq.hpp>

/**
 * @file zmq_io.h
 * @author Group 7 - Informatica
 */

 /**
  * @brief Protocol to be used for message transmission
  */
const std::string PROTOCOL = "tcp";

namespace goliath::messaging {
    /**
     * @class goliath::messaging::ZmqIo
     * @brief Base class for zmq objects
     */
    class ZmqIo {
    public:
        /**
         * @param context Zmq context
         * @param host Connect/bind host
         * @param port Connect/bind port
         */
        ZmqIo(zmq::context_t &context, const std::string &host, const int port)
                : ZmqIo(context, host, port, 0) {}

        /**
         * @param context Zmq context
         * @param host Connect/bind host
         * @param port Connect/bind port
         * @param socketType ZMQ type of socket e.g. PUB/SUB
         */
        ZmqIo(zmq::context_t &context, const std::string &host, const int port, const int socketType)
                : context(context), host(host), port(port), socket(context, socketType) {}

        /**
         * @return Formatted address
         */
        std::string address() const;

        /**
         * @brief Connect to address
         */
        void connect();
        /**
         * @brief Bind to port
         */
        void bind();

    protected:
        zmq::context_t& context;
        const std::string host;
        const int port;
        zmq::socket_t socket;
    };
}
