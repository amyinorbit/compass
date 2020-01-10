add_library(CompassCompiler STATIC compiler.cpp parser.cpp infer.cpp)
target_link_libraries(CompassCompiler PUBLIC apfun::apfun CompassLanguage CompassRT2)
target_include_directories(CompassCompiler INTERFACE ${PROJECT_SOURCE_DIR}/include)
