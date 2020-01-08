add_library(CompassLanguage STATIC grammar.cpp lexer.cpp rdparser.cpp)
target_include_directories(CompassLanguage INTERFACE ${PROJECT_SOURCE_DIR}/include)
