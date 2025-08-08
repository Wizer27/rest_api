cmake_minimum_required(VERSION 3.10)
project(my_api)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    /Users/ivan/rest_api/pistache/include
)

link_directories(
    /Users/ivan/rest_api/pistache/build/src
    /opt/homebrew/lib
)

add_executable(my_api api.cpp)

# ⚠️ Добавляем и event, и event_pthreads
target_link_libraries(my_api
    /Users/ivan/rest_api/pistache/build/src/libpistache.a
    event
    event_pthreads
    pthread
)
