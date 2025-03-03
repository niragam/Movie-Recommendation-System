FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
        cmake \
        g++ \
        git \
        wget \
        libpthread-stubs0-dev \
        python3 \
        && apt-get clean

WORKDIR /usr/src/project

COPY . .

RUN mkdir -p build && cd build && \
    cmake .. && \
    make

CMD ["/bin/bash"]