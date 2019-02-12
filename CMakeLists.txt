cmake_minimum_required(VERSION 3.2)
project(Compass VERSION 3.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(src)

add_custom_target(run
    COMMAND compassr test.txt
    DEPENDS compassr
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(debug
    COMMAND lldb compassr test.txt
    DEPENDS compassr
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

