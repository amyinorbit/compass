add_library(CompassRT2 STATIC function.cpp memory.cpp collector.cpp type.cpp unpack.cpp)
target_link_libraries(CompassRT2)
target_include_directories(CompassRT2 INTERFACE ${PROJECT_SOURCE_DIR}/include)
