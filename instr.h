#ifndef INSTRU_H
#define INSTRU_H
#include <stdint.h>
#include <string>
#include <fstream>
#include "dis.h"

using namespace std;

class instru{

  public:

  uint8_t opcode, pos;
  int64_t operand1, operand2;

  //Stores if operand1/2 are registers
  bool operand1r, operand2r;

  //Stores operand size (used if not register)
  int operandSize;

  //True is operands are read in reverse order
  bool swapOperands;

  string mne, reg;
  bool twoOps;

  //REX prefix variables
  bool rex, rexW, rexR, rexX, rexB;

  //Variable length opcode prefixes
  bool opPre1, opPre2;

  bool modRegRM;

  instru(){

    opcode = 0;
    operand1 = operand2 = 0;
    twoOps = false;

    operand1r = operand2r = 0;

    swapOperands = 0;

    operandSize = 0;

    rex = rexW = rexR = rexX = rexB = 0;

    opPre1 = opPre2 = 0;

    modRegRM = 0;
  }

  bool getInstru(istream &file, int readPos){
    //Getting instruction 2
    pos = readPos;

    if (parsePrefix(file, pos)){
      //Increment pos if a prefix was read
      pos++;
      //Increment pos is a second prefix was read
      if(opPre2)
        pos++;
    }

    //Next byte should be the opcode
    if (!(opcode = getByte(file, pos++))){
      cout << "READ INVALID OPCODE" << endl;
      return false;
    }

    cout << "OPCODE: " << hex << +opcode << endl;

    parseOpcode(opcode, file);

    if (!modRegRM && !opPre1){
      if (operand1r)
        //Get 1 byte then increment pos by 1
        operand1 = getByte(file,pos++);
      else{
        //Get 4 bytes then increment pos by 4
        operand1 = get4Bytes(file, pos);
        pos += 4;
      }
      cout << "OPER1: " << operand1 << endl;

      if (operand2r)
        //Get 1 byte then increment pos by 1
        operand2 = getByte(file,pos++);
      else{
        //Get 4 bytes then increment pos by 4
        operand2 = get4Bytes(file, pos);
        pos += 4;
      }

      cout << "OPER2: " << hex << +operand2 << endl;
    }

    if (swapOperands){
      int64_t tempOperand = operand1;
      operand1 = operand2;
      operand2 = tempOperand;

      bool tempr = operand1r;
      operand1r = operand2r;
      operand2r = tempr;
    }

    //Print instruction to cout
    cout << "\t" << mne << "\t";

    if (operand1r)
      cout << getReg(operand1);
    else
      cout << "$0x" << hex << +operand1;

    cout << ",";

    if (operand2r)
      cout << getReg(operand2);
    else
      cout << "$0x" << hex << +operand2;

    cout << endl;

    pos = file.tellg();

    //End function
    return true;

  
  }

  bool parsePrefix(istream &file, int pos){
    uint8_t prefix = getByte(file, pos);

    //Check for REX prefix
    if ( (prefix & 0xF0) == 0x40){
      rex = 1;

      //REX.W
      if ( (prefix & 0x8) == 0x8)
        rexW = 1;
      //REX.R
      if ( (prefix & 0x4) == 0x4)
        rexR = 1;
      //REX.X
      if ( (prefix & 0x2) == 0x2)
        rexX = 1;
      //REX.B
      if ( (prefix & 0x1) == 0x1)
        rexB = 1;

      return true;
    }

    //Check to see if the byte read is a prefix
    switch(prefix){
      //Group 1
      case 0xF0:   opPre1 = prefix;
		   opPre2 = getByte(file, pos + 1);

                   if (opPre2 != 0x38 && opPre2 != 0x3A)
                     opPre2 = 0;
                  return true;
      case 0xF2:
      case 0xF3:
      //Group 2
      case 0x2E:
      case 0x36:
      case 0x3E:
      case 0x26:
      case 0x64:
      case 0x65:
      //Group 3
      case 0x66:
      //Group 4
      case 0x67: cout << "Prefix not yet implemented" << endl;
                 return true;
      //No prefix found
      default: return false;
    }
  }



  bool parseOpcode(uint8_t opcode, istream &file){
    if (opPre1 == 0x0F){
      switch(opcode)
        case 0x05: mne = "syscall";
                   return true;
    }

    switch(opcode){
      //mov instruction	0xc7
      case 0xc7: mne = "mov";
		 //Operands are 4 bytes
		 operandSize = 4;
		 //Operand1 is a register
		 //operand1r = 1;
		 //Operands are printed in the reverse order
		 swapOperands = 1;
		 
		 parseRegModRM(getByte(file, pos++));
		 operand2r = 0;
        	 operand2 = get4Bytes(file, pos);

		 return true;
      //mov instruction 0x89
      case 0x89: mne = "mov";
		 parseRegModRM(getByte(file, pos++));
		 return true;

      //Return false for unknown operation
      default:   mne = "UNK_OP";
		 return false;
    }
  }

  void parseRegModRM(uint8_t byte){
    modRegRM = 1;

    cout << "MOD REG R/M: " << hex << +byte << endl;

    bool d = opcode & 0x01;

    cout << "d: " << d << endl;

    uint8_t mod = byte >> 6;
    uint8_t reg = (byte & 0x38) >> 3;
    uint8_t rm = byte & 0x07;

    if (mod == 0b11){
     operand1r = operand2r = 1; 
     operand1 = reg;
     operand2 = rm;

     cout << hex << operand1 << endl;


    }

  }



  string getReg(uint8_t reg){

    switch(reg){

      case 0x0: return "%rax";
      case 0x1: return "%rcx";
      case 0x2: return "%rdx";
      case 0x3: return "%rbx";
      case 0x4: return "%rsp";
      case 0x5: return "%rbp";
      case 0x6: return "%rsi";
      case 0x7: return "%rdi";

      //%rax
      case 0xc0: return "%rax";

      //Return UNK for unknown register
      default:   return "UNK";
    }
  }


  void clearInstru(){
    operand1 = 0; 
    operand2 = 0;
    rex = twoOps = false;
  }
};

#endif
