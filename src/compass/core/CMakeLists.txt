add_library(CompassCore STATIC story.cpp utf8.c verb.cpp writer.cpp)
target_include_directories(CompassCore INTERFACE ${PROJECT_SOURCE_DIR}/include)
