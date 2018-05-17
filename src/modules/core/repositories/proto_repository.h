#pragma once

#include "repository.h"

/**
 * @file vision_repo.h
 * @brief repository for accessing proto objects.
 * @author Group 7 - Informatica
 */
namespace goliath::repositories {
    template<typename T>
    class ProtoRepository : public Repository {
    public:
        explicit ProtoRepository(T *proto)
            : proto(proto) { }

        /**
         * Merges a protobuf into this repository.
         * @param proto proto to merge from into this repo
         */
        virtual void merge_from_message(T *proto) = 0;

        /**
         * @return the protobuf contained in this repository
         */
        std::unique_ptr<::google::protobuf::Message> getMessage() override;
    protected:
        T *proto;
    };

    template<typename T>
    std::unique_ptr<::google::protobuf::Message> ProtoRepository<T>::getMessage() {
        return std::make_unique<T>(*proto);
    }
}
