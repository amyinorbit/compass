add_library(Compass STATIC
	lexer.cpp utf8.c grammar.cpp storyparser.cpp storyparser+rd.cpp story.cpp semantics.cpp
)
target_include_directories(Compass INTERFACE ${PROJECT_SOURCE_DIR}/include)
