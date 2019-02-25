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
- [ ] make objects unique per container, "allow sofa to appear in different places"
- [ ] better compiler messaging
- [ ] compiler qt gui?
- [ ] properties system (move to bytecode later?)
	- [ ] lock support
- [ ] adjectives system
- [ ] better game loop architecture
- [ ] better error reporting system, plain english errors
- [ ] better object/object matching system
- [ ] custom actions & bytecode support
