#include "goliath/zmq_publisher.h"

#include <unistd.h>

using namespace goliath::io;

int main(int argc, char *argv[]) {
    // "You should create and use exactly one context in your process."
    zmq::context_t context(1);

    zmq_publisher pub(context, "127.0.0.1", 5556);

    // Give 0MQ/2.0.x time to connect
    usleep(2000);

    // Let's publish a few messages
    for (size_t i = 0; i < 10; ++i) {
        Message msg;

        if (i < 5) {
            auto *movement(new MoveCommand);
            movement->set_speed(123);
            movement->set_direction(123);

            msg.set_allocated_movecommand(movement);
        } else {
            auto *ioConfig(new IoConfig);
            ioConfig->set_publisher_ip("127.0.0.1");
            ioConfig->set_publisher_port(5556);

            msg.set_allocated_ioconfig(ioConfig);
        }

        pub.publish(msg);
    }

    // Give 0MQ/2.0.x time to publish
    usleep(1000);

    return 0;
}