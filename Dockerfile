FROM alpine:3.7

ENV PKG="build-base \
         ca-certificates \
         g++ \
         gcc \
         make \
         git \
         cmake \
         clang \
         clang-dev \
         libc-dev \
         libstdc++ \
         pkgconf \
         linux-headers \
         boost \
         boost-dev \
         libzmq \
         zeromq-dev \
         protobuf \
         protobuf-dev \
         openal-soft \
         openal-soft-dev \
         opencv \
         opencv-libs \
         opencv-dev"

# Add edge repo
RUN echo 'http://dl-cdn.alpinelinux.org/alpine/edge/main'  >> /etc/apk/repositories && \
    echo 'http://dl-cdn.alpinelinux.org/alpine/edge/community' >> /etc/apk/repositories && \
    echo 'http://dl-cdn.alpinelinux.org/alpine/edge/testing'  >> /etc/apk/repositories

# Update packages
RUN apk upgrade --update --no-cache && \
    # Install packages
    apk add --update --no-cache ${PKG}

# Clone cppzmq
RUN mkdir /tmp/cppzmq
WORKDIR /tmp/cppzmq
RUN git clone https://github.com/zeromq/cppzmq.git cppzmq-master && \
    mkdir /tmp/cppzmq/cppzmq-master/build

# Build / install cppzmq
WORKDIR /tmp/cppzmq/cppzmq-master/build
RUN cmake \
    -D CPPZMQ_BUILD_TESTS=OFF .. && \
    make -j4 install && \
    rm -rf /tmp/cppzmq

VOLUME /core
WORKDIR /core

ENTRYPOINT ./docker-entrypoint.sh