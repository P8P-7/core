#pragma once

#include <google/protobuf/message.h>

/**
 * @file repository.h
 * @brief repository base class
 * @author Group 7 - Informatica
 */
namespace goliath::repositories {
    class Repository {
    public:
        /**
         * Compiles the fields of this repository to an instance of ::google::protobuf::Message.
         * @return instance of ::google::protobuf::Message with all information
         */
        virtual std::unique_ptr<::google::protobuf::Message> getMessage() = 0;

        /**
         * @return true if this repository has been changed
         */
        virtual bool isInvalidated() {
            return invalidated;
        }

        /**
         * Forces the repository to be synchronized
         */
        void invalidate() {
            invalidated = true;
        }
    private:
        friend class Watcher;

        void validate() {
            invalidated = false;
        }

        bool invalidated = false;
    };
}
