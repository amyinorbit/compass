add_library(CompassType STATIC kind.cpp object.cpp context.cpp)
target_link_libraries(CompassType)
target_include_directories(CompassType INTERFACE ${PROJECT_SOURCE_DIR}/include)
