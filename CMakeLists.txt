cmake_minimum_required(VERSION 3.10)
project(my_api)

set(CMAKE_CXX_STANDARD 17)

# Указываем путь к nlohmann-json, установленному с помощью vcpkg
find_package(nlohmann_json CONFIG REQUIRED)

include_directories(
    /Users/ivan/rest_api/pistache/include
)

link_directories(
    /Users/ivan/rest_api/pistache/build/src
    /opt/homebrew/lib
)

add_executable(my_api api.cpp)

# Линкуем с библиотеками
target_link_libraries(my_api
    /Users/ivan/rest_api/pistache/build/src/libpistache.a
    event
    event_pthreads
    pthread
    nlohmann_json::nlohmann_json  # Линкуем nlohmann-json
)
