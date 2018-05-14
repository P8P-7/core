#include "zmq_publisher.h"

#include <unistd.h>

using namespace goliath::io;

int main(int argc, char *argv[]) {
    // "You should create and use exactly one context in your process."
    zmq::context_t context(1);

    goliath::io::zmq_publisher pub(context, "127.0.0.1", 5556);

    // Give 0MQ/2.0.x time to connect
    usleep(2000);

    // Let's publish a few messages
    for (size_t i = 0; i < 10; ++i) {
        MessageCarrier msg;
        CommandMessage *command = new CommandMessage;

        MoveCommand *movement = new MoveCommand;
        movement->set_speed(123);
        movement->set_direction(123);

        command->set_allocated_movecommand(movement);

        msg.set_allocated_commandmessage(command);

        pub.publish(msg);
    }

    // Give 0MQ/2.0.x time to publish
    //usleep(1000);

    return 0;
}
