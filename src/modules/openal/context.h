#pragma once

#include "device.h"

/**
 * @file context.h
 * @author Group 7 - Informatica
 */

 namespace goliath::openal {
     /**
      * @brief Context to be passed to different devices
      */
     class context {
     public:
         /**
          * @param device Device to set as main device within context
          * @param context Context address (Default NULL)
          */
         context(const device &device, const ALCint *context);

     private:
        ALCcontext* alc_context;
     };
 }
