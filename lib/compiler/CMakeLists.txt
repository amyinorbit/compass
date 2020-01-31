add_library(CompassCompiler STATIC
    assembler.cpp
    compiler.cpp
    parser.cpp
    infer.cpp
    type.cpp
    codegen.cpp
)
target_link_libraries(CompassCompiler PUBLIC apfun::apfun CompassLanguage CompassRT2)
target_include_directories(CompassCompiler INTERFACE ${PROJECT_SOURCE_DIR}/include)
