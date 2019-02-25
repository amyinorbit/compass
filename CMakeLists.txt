cmake_minimum_required(VERSION 3.2)
project(Compass VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(DEV_VERSION_POSTFIX "dr11")
set(REL_VERSION_POSTFIX "r1")

set(PRODUCT_DIR "${CMAKE_SOURCE_DIR}/product")
set(EXAMPLE_ASSET "${CMAKE_SOURCE_DIR}/example.txt")
set(VERSION_TPL "${CMAKE_SOURCE_DIR}/version.txt")
set(VERSION_OUT "${PRODUCT_DIR}/version.txt")
set(CMAKE_INSTALL_PREFIX ${PRODUCT_DIR})


if(CMAKE_BUILD_TYPE MATCHES RELEASE)
	set(PROJECT_VERSION "${PROJECT_VERSION}${REL_VERSION_POSTFIX}")
	add_definitions( -DCOMPASS_VERSION_STRING=${PROJECT_VERSION} )
else()
	set(PROJECT_VERSION "${PROJECT_VERSION}${DEV_VERSION_POSTFIX}")
	add_definitions( -DCOMPASS_VERSION_STRING=\"${PROJECT_VERSION}\" )
endif()

configure_file(${VERSION_TPL} ${VERSION_OUT})

find_package(Boost REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

add_subdirectory(lib)
add_subdirectory(bin)

add_custom_target(run
    COMMAND compass-run
    DEPENDS compass-run
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

