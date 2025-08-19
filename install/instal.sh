#!/bin/bash

# Для Ubuntu/Debian
if command -v apt &> /dev/null; then
    sudo apt update
    sudo apt install -y \
        build-essential \
        cmake \
        pkg-config \
        libpistache-dev \
        libevent-dev \
        nlohmann-json3-dev

# Для macOS с Homebrew
elif command -v brew &> /dev/null; then
    brew update
    brew install \
        cmake \
        pkg-config \
        pistache \
        libevent \
        nlohmann-json

else
    echo "Неизвестный пакетный менеджер"
    exit 1
fi