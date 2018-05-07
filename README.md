# x86_64-Disassembler
[![Build Status](https://travis-ci.org/JonathanPugh/x86_64-Disassembler.svg?branch=master)](https://travis-ci.org/JonathanPugh/x86_64-Disassembler)
This is a barebones x86_64 disassembler that supports a limited number of instructions. Output is printed to stdout and adheres to GAS syntax (a combination of AT&T and Intel syntaxes).

## Compiling
Compile with g++:

```
g++ *.cpp
```

## Supported instructions
The following opcodes are supported:
```
mov : c7, 89, B*
add : 01, 83
```

## Supported formats
Operations of the following formats are supported:
```
opcode imm32, r64
opcode imm32, r32
opcode imm8, r64
opcode imm8, r32
opcode r64, r64
opcode r64, r32
```
