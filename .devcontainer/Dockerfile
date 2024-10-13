FROM mcr.microsoft.com/devcontainers/cpp:1.2.2-debian-12
RUN sudo apt update && sudo apt upgrade -y
RUN sudo apt install -y gcc gdb cmake ninja-build
RUN git clone https://github.com/google/googletest.git -b v1.15.2 &&\
  cd googletest && mkdir build && cd build &&\
  cmake .. && make && sudo make install &&\
  cd ~/ && rm -fr googletest
