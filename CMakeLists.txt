cmake_minimum_required(VERSION 3.2)
project(house VERSION 3.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(src)

add_custom_target(run
    COMMAND house_test
    DEPENDS house_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
