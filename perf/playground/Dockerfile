FROM gcc:9
LABEL authors="Zach.DeGrey"

WORKDIR /src

COPY main.cpp .
COPY CMakeLists.txt .

RUN set -ex; \
    apt-get update; \
    apt-get install -y cmake; \
    apt-get install -y gdb; \
    cd /src; \
    cmake .; \
    cmake --build .

CMD [ "/src/AlgoGaugePerf" ]