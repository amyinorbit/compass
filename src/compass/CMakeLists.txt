add_library(Compass STATIC
	lexer.cpp
	utf8.c
	grammar.cpp
	rdparser.cpp
	sentence.cpp
	story.cpp
	run.cpp
	game.cpp
	parser.cpp
	sema.cpp
	verb.cpp
	compiler.cpp
)
target_include_directories(Compass INTERFACE ${PROJECT_SOURCE_DIR}/include)
