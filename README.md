# x86_64 Disassembler
[![Build Status](https://travis-ci.org/JonathanPugh/x86_64-Disassembler.svg?branch=master)](https://travis-ci.org/JonathanPugh/x86_64-Disassembler)

This is a barebones x86_64 GNU/Linux ELF binary disassembler that supports a limited number of instructions. Output is printed to stdout and adheres to the GAS syntax.

Examples to test disassembly can be found in simpleTest/ and helloWorld/

## Compiling
Compile the disassembler with g++:

```
g++ *.cpp -o dis
```

Compile the tests with gcc:
```
gcc -nostdlib simpleTest/test.s
gcc -nostdlib helloWorld/helloWorld.s
```

## Usage
Pass the path of the executable to disassemble as a command line argument:

```
./dis pathToExecutable
```
If "pathToExecutable" is not specified, the disassembler will try to open "a.out" by default.

## Supported instructions
The following opcodes are supported:
```
Mneumonic    Opcode     Operand Format

mov        | 89      |  r32/64, r32/64
           | c7      |   imm32, r32/64
           | B*      |   imm32, r32/64 

add        | 01      |  r32/64, r32/64
           | 83      |    imm8, r32/64

push       | 68      |   imm32

(i)mul     | F7      |  r32/64

(i)div     | F7      |  r32/64

syscall    | 0F 05 
```

## Features
The disassembler is capable of the following:
 * Verifying the ELF header
 * Detecting the achitecture the binary uses (32 or 64 bit)
 * Parsing prefix byte(s) such as the REX prefix
 * Differentiating between 32bit and 64bit registers
 * Parsing the opcode of supported instructions
 * Parsing the Reg-Mod-RM byte

## Limitations
 * Any instruction or format not listed above is not supported
 * .data segment declarations are not supported
 * Any labels other than _start: are not supported
