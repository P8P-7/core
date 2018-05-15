#include "context.h"

using namespace goliath::audio;
using namespace goliath::exceptions;

context::context(const device &device, const ALCint *context) {
    alc_context = alcCreateContext(device.get_device(), context);

    if(alc_context == NULL) {
        throw new openal_error("Unable to create context");
    }

    alcMakeContextCurrent(alc_context);
    alcProcessContext(alc_context);
}
