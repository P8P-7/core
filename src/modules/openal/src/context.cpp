#include <goliath/openal/context.h>
#include <goliath/openal/util/openal_error.h>

using namespace goliath::openal;
using namespace goliath::exceptions;

Context::Context(const Device &device, const ALCint *context) {
    alcContext = alcCreateContext(device.getDevice(), context);

    if(alcContext == nullptr) {
        throw OpenalError("Unable to create context");
    }

    alcMakeContextCurrent(alcContext);
    alcProcessContext(alcContext);
}
