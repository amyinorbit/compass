// generated by gen_code.py
#pragma once


OPCODE(halt      , 0x00,  +0,  +0) // halts the virtual machine
OPCODE(loadc     , 0x01,  +2,  +1) // pushes a constant on the stack from a value from the pool
OPCODE(loadg     , 0x02,  +2,  +1) // pushes a global on the stack
OPCODE(loadl     , 0x03,  +1,  +1) // pushes a local variable onto the stack
OPCODE(loada     , 0x04,  +0,  -1) // pops an array reference and index, and pushes the array item
OPCODE(storeg    , 0x05,  +4,  -1) // pops a value from the stack into a global
OPCODE(storel    , 0x06,  +1,  -1) // pops a value from the stack into a local
OPCODE(storea    , 0x07,  +0,  -3) // pops a value into an array slot
OPCODE(drop      , 0x08,  +0,  -1) // removes the top-of-stack.
OPCODE(dup       , 0x09,  +0,  +1) // duplicates the item on TOS.
OPCODE(resv      , 0x0a,  +2,  +1) // reserves local variables by shifting the stack pointer
OPCODE(jmp       , 0x0b,  +2,  +0) // jumps by n addresses
OPCODE(rjmp      , 0x0c,  +2,  +0) // jumps back by n addresses
OPCODE(jmpz      , 0x0d,  +2,  +0) // jumps forward n addresses if TOS == 0
OPCODE(rjmpz     , 0x0e,  +2,  +0) // jumps back n addresses if TOS == 0
OPCODE(jmpnz     , 0x0f,  +2,  +0) // jumps forward n addresses if TOS != 0
OPCODE(rjmpnz    , 0x10,  +2,  +0) // jumps back n addresses if TOS != 0
OPCODE(call      , 0x11,  +2,  -1) // calls a function with n arguments
OPCODE(ret       , 0x12,  +0,  +0) // returns from a function
OPCODE(ioselect  , 0x13,  +2,  +0) // selects an I/O device ID
OPCODE(iowrite   , 0x14,  +0,  -1) // writes the top of stack to the IO device
OPCODE(ioread    , 0x15,  +0,  +1) // reads a line of text from the IO device onto the stack
OPCODE(iostyle   , 0x16,  +2,  +0) // sets the IO device style
OPCODE(parse     , 0x17,  +2,  +0) // s
OPCODE(i2s       , 0x18,  +0,  +0) // converts the TOS from integer to string
OPCODE(i2f       , 0x19,  +0,  +0)
OPCODE(f2s       , 0x1a,  +0,  +0)
OPCODE(f2i       , 0x1b,  +0,  +0)
OPCODE(addi      , 0x1c,  +0,  -1) // adds two integers
OPCODE(subi      , 0x1d,  +0,  -1) // subtracts an integer from another
OPCODE(muli      , 0x1e,  +0,  -1) // multiplies two integers
OPCODE(divi      , 0x1f,  +0,  -1) // divides an integer by another
OPCODE(cmpi      , 0x20,  +0,  -1) // compares two integers
OPCODE(addf      , 0x21,  +0,  -1) // adds two floats
OPCODE(subf      , 0x22,  +0,  -1) // subtracts an float from another
OPCODE(mulf      , 0x23,  +0,  -1) // multiplies two floats
OPCODE(divf      , 0x24,  +0,  -1) // divides an float by another
OPCODE(cmpf      , 0x25,  +0,  -1) // compares two floats
OPCODE(cmps      , 0x26,  +0,  -1) // compares two strings

