#include "goliath/zmq_publisher.h"

#include <unistd.h>

using namespace goliath::io;

int main(int argc, char *argv[]) {
    // "You should create and use exactly one context in your process."
    zmq::context_t context(1);

    zmq_publisher pub(context, "127.0.0.1", 5556);

    // Give 0MQ/2.0.x time to connect
    usleep(2000);

    // Let's publish a few "Hello" messages
    for (size_t i = 0; i < 10; ++i) {
        std::stringstream s;

        s << "Hello " << i;

        CommandMessage msg;
        msg.set_channel(CommandMessage_Channel_DEFAULT);

        auto *command(new Command);

        if (i < 5) {
            auto *movement(new MoveCommand);
            movement->set_speed(123);
            movement->set_direction(123);

            command->set_allocated_movecommand(movement);
        } else {
            auto *config(new ConfigCommand);
            config->set_name("test");
            config->set_value("test");

            command->set_allocated_configcommand(config);
        }

        msg.set_allocated_command(command);

        pub.publish(msg);
    }

    // Give 0MQ/2.0.x time to publish
    usleep(1000);

    return 0;
}
