add_library(Compass STATIC
	lexer.cpp
	utf8.c
	grammar.cpp
	# storyparser.cpp
	rdparser.cpp
	sentence.cpp
	story.cpp
	# semantics.cpp
	# run.cpp
	# game.cpp
	parser.cpp
	contextsema.cpp
)
target_include_directories(Compass INTERFACE ${PROJECT_SOURCE_DIR}/include)
