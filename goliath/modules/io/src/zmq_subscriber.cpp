#include <goliath/zmq_subscriber.h>

using namespace goliath::io;

zmq_subscriber::zmq_subscriber(zmq::context_t &context, const std::string &host, const int port,
                               const std::string &topic)
        : zmq_io(context, host, port, ZMQ_SUB), topic(topic) {
    connect(address());
    socket.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());

    // To use zmq_poll correctly, we construct this vector of poll items
    p = {{socket, 0, ZMQ_POLLIN, 0}};

    // The subscriber thread runs until it received all of the messages
    subs_thread = std::thread(&zmq_subscriber::receive, this);
}

void zmq_subscriber::receive() {
    while (!stop_flag) {
        zmq::message_t address;
        zmq::message_t data;

        try {
            // When timeout (the third argument here) is -1,
            // then block until ready to receive
            zmq::poll(p.data(), p.size(), -1);

            if (p[0].revents & ZMQ_POLLIN) {
                //  Read envelope with address
                socket.recv(&address);

                std::string topic = std::string(static_cast<char *>(address.data()), address.size());

                //  Read message contents
                socket.recv(&data);

                idp::Content content;
                content.ParseFromArray(data.data(), static_cast<int>(data.size()));

                std::cout << "[" << topic << "] " << content.content() << std::endl;
            }
        }
        catch (const zmq::error_t &ex) {
            // recv() throws ETERM when the zmq context is destroyed,
            //  as when socket.close() is called
            if (ex.num() != ETERM) {
                break;
            }

        }
    }
}

void zmq_subscriber::stop() {
    socket.close();
    stop_flag = true;
    subs_thread.join();
}




