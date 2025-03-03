FROM gcc:latest

RUN apt-get update && apt-get install -y cmake build-essential && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/project

COPY . .
COPY data/ /usr/src/project/data/

RUN mkdir -p build
WORKDIR /usr/src/project/build
RUN cmake ..
RUN make
RUN chmod +x movieApp testsMovies

ENTRYPOINT ["sh", "-c"]
CMD ["./movieApp"]
