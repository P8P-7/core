#include "zmq_subscriber.h"
#include <unistd.h>

using namespace goliath::io;

zmq_subscriber::zmq_subscriber(zmq::context_t &context, const std::string &host, const int port,
                               const std::string &topic)
        : zmq_io(context, host, port, ZMQ_SUB), topic(topic) {
    connect(address());
    socket.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());

    // To use zmq_poll correctly, we construct this vector of poll items
    poll = {
            {nullptr, 0, ZMQ_POLLIN, 0}, // Not polling a ZMQ socket, poll on stdin. 0 is the fd for stdin.
            {socket,  0, ZMQ_POLLIN, 0} // Polling on a ZMQ socket
    };

    // The subscriber thread runs until it received all of the messages
    subs_thread = std::thread(&zmq_subscriber::receive, this);
}

void zmq_subscriber::receive() {
    while (!interrupted) {
        zmq::message_t address;
        zmq::message_t data;

        // When timeout (the third argument here) is -1,
        // then block until ready to receive
        zmq::poll(poll.data(), poll.size(), -1);

        if (poll[0].revents == ZMQ_POLLIN) {
            // A key has been pressed, read it
            char c;
            read(0, &c, 1);

            if (c == 'c') {
                std::cout << "Interrupt received, will exit…" << std::endl;
                interrupted = true;
            }
        }
        if (poll[1].revents == ZMQ_POLLIN) {
            //  Read envelope with address
            socket.recv(&address);

            std::string topic = std::string(static_cast<char *>(address.data()), address.size());

            //  Read message contents
            socket.recv(&data);

            Command command;
            command.ParseFromArray(data.data(), static_cast<int>(data.size()));

            switch (command.command_case()) {
                case Command::kMoveCommand: {
                    const MoveCommand &moveCommand = command.movecommand();

                    std::cout << "[" << topic << "] [ move ] speed: "
                              << moveCommand.speed()
                              << " direction: "
                              << moveCommand.direction() << std::endl;
                    break;
                }
                case Command::kConfigCommand: {
                    const ConfigCommand &configCommand = command.configcommand();

                    std::cout << "[" << topic << "] [ config ] name: "
                              << configCommand.name()
                              << " value: "
                              << configCommand.value() << std::endl;
                    break;
                }
                case Command::COMMAND_NOT_SET: {
                    std::cout << "Command not set" << std::endl;
                    break;
                }
            }
        }
    }
}

void zmq_subscriber::join() {
    subs_thread.join();
}
