#ifndef DIS_H
#define DIS_H

#include <stdint.h>
#include <fstream>

using namespace std;

//Address of the entrypoint in the ELF header
const uint8_t ENTRYPOINT = 0x18;

//Protoypes for functions defined in ops.cpp
uint8_t getByte(istream &, int);

int32_t get4Bytes(istream &, int);

bool verifyHeader(istream &);

bool verifyArch(istream &);

long int entryPoint (istream &);
#endif
