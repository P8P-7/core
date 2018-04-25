#include <goliath/caterpillar_movement.h>
#include <goliath/io.h>

#include "commands/move_command.h"

#include <goliath/message.pb.h>

#include <unistd.h>

using namespace goliath::core;
using namespace goliath::io;

int main(int argc, char *argv[]) {
    // "You should create and use exactly one context in your process."
    zmq::context_t context(1);

    const std::string TOPIC = "default";

    zmq_subscriber sub(context, "127.0.0.1", 5555, TOPIC);
    zmq_publisher pub(context, "127.0.0.1", 5556);

    // Give 0MQ/2.0.x time to subscribe
    usleep(2000);

    // Let's publish a few "Hello" messages
    for (size_t i = 0; i < 10; ++i) {
        std::stringstream s;

        s << "Hello " << i;

        auto *data(new idp::Content);
        data->set_content(s.str());

        idp::Message msg;
        msg.set_channel(TOPIC);

        // Takes ownership of data -
        // you don't call 'delete data'
        msg.set_allocated_data(data);

        pub.publish(msg);
    }

    // Give 0MQ/2.0.x time to publish
    usleep(1000);

    sub.stop();

    return 0;
}