#include "goliath/zmq_subscriber.h"

#include <unistd.h>

#include "handles/handle_map.h"

using namespace goliath::handles;
using namespace goliath::io;

int main(int argc, char *argv[]) {
    // "You should create and use exactly one context in your process."
    zmq::context_t context(1);

    const std::string TOPIC = std::to_string(Message::DataCase::kCommand);

    zmq_subscriber sub(context, "127.0.0.1", 5555, TOPIC);

    sub.join();

    return 0;
}
