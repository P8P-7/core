#pragma once

#include "device.h"

/**
 * @file context.h
 * @author Group 7 - Informatica
 */

 namespace goliath::openal {
     /**
      * @class goliath::openal::Context
      * @brief Context to be passed to different devices
      */
     class Context {
     public:
         /**
          * @param device Device to set as main device within context
          * @param context Context address (Default NULL)
          */
         Context(const Device &device, const ALCint *context);

     private:
        ALCcontext* alcContext;
     };
 }
