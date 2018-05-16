#include "context.h"

using namespace goliath::openal;
using namespace goliath::exceptions;

Context::Context(const Device &device, const ALCint *context) {
    alcContext = alcCreateContext(device.getDevice(), context);

    if(alcContext == NULL) {
        throw openal_error("Unable to create context");
    }

    alcMakeContextCurrent(alcContext);
    alcProcessContext(alcContext);
}
