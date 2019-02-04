add_library(House STATIC lexer.cpp utf8.c grammar.cpp storyscript.cpp storyscript+rd.cpp story.cpp)
target_include_directories(House INTERFACE ${PROJECT_SOURCE_DIR}/include)
