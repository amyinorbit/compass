add_library(CompassCompiler STATIC compiler.cpp parser.cpp sema.cpp)
target_link_libraries(CompassCompiler CompassCore CompassLanguage CompassFilesystem)
target_include_directories(CompassCompiler INTERFACE ${PROJECT_SOURCE_DIR}/include)
