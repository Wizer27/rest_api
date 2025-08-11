cmake_minimum_required(VERSION 3.10)

# Указываем название проекта
project(my_api)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)

# Находим библиотеку nlohmann-json
find_package(nlohmann_json CONFIG REQUIRED)

# Добавляем пути к заголовочным файлам для других библиотек
include_directories(
    /Users/ivan/rest_api/pistache/include
)

# Добавляем пути к директориям для линковки
link_directories(
    /Users/ivan/rest_api/pistache/build/src
    /opt/homebrew/lib
)

# Создаем исполнимый файл
add_executable(my_api src/api.cpp)

# Линкуем с нужными библиотеками
target_link_libraries(my_api
    /Users/ivan/rest_api/pistache/build/src/libpistache.a
    event
    event_pthreads
    pthread
    nlohmann_json::nlohmann_json  # Линковка с nlohmann-json
)
