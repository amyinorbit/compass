## Compass Engine

Compass is an experimental engine for interactive fiction. It's heavily inspired by Inform7, but I wanted something simpler in some ways, different in others, and mostly liked the challenge of writing a parser for this. It's very early in the development still, and I wouldn't recommend anyone rely on the syntax not changing, but feel free to experiment.

## Immediate roadmap

- [x] default verb enabling
- [x] in/on/under location support
- [x] in/on/under location display
- [x] include system
- [x] project structure overhaul
- [x] compilation/loader system
	- [x] basic types writing
	- [x] file writing
	- [x] file reading
- [x] compiler / runtime architecture switch
- [x] better name recognition (don't fail on preposition in the middle of names)
- [x] better compiler messaging
- [x] add support for defining synonyms
- [ ] overhaul runtime system
    - [ ] add support for classes/methods/objects
    - [ ] add support for Number, String and Boolean properties. Arrays?
    - [ ] implement standard verbs as function objects
    - [ ] implement stack/object graph?
    - [ ] move story file writing to v2.0 object graph
- [ ] Add bytecode support

- [ ] compiler gui? QT?
- [ ] replace json project manifest with custom syntax?
- [ ] better error reporting system, plain english errors
