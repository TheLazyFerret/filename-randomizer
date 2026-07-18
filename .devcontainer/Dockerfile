FROM fedora:latest

RUN dnf update -y && \
    dnf install -y gcc g++ clang clang-tools-extra cmake make gdb valgrind git curl vim && \
    dnf clean all

ENV CC="clang" \
    CXX="clang++" \
    CMAKE_GENERATOR="Unix Makefiles"