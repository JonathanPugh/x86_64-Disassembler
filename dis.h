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

class instru{

  public:

  uint8_t opcode;
  uint64_t pos;
  int64_t operand1, operand2;

  //Stores if operand1/2 are registers
  bool operand1r, operand2r;

  //Stores operand size (used if not register)
  int operandSize;

  //True is operands are read in reverse order
  bool swapOperands;

  string mne, reg;
  bool noOps, oneOp;

  //REX prefix variables
  bool rex, rexW, rexR, rexX, rexB;

  //Variable length opcode prefixes
  uint8_t opPre1, opPre2;

  uint8_t RM_reg, RM_mod, RM_rm;

  bool modRegRM;
 
  instru();

  bool getInstru(istream &, int);
  bool parsePrefix(istream &, int);
  bool parseOpcode(uint8_t, istream &);
  void parseRegModRM(uint8_t);
  string getReg(uint8_t);
  void clearInstru();
};
#endif
