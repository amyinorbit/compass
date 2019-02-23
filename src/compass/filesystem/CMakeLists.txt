add_library(CompassFilesystem STATIC path.cpp)
target_include_directories(CompassFilesystem INTERFACE ${PROJECT_SOURCE_DIR}/include)
