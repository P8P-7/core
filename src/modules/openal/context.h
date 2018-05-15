#pragma once

#include "device.h"

/**
 * @file context.h
 * @author Group 7 - Informatica
 */

 namespace goliath::audio {
     class context {
     public:
         context(const device &device, const ALCint *context);

     private:
        ALCcontext* alc_context;
     };
 }
