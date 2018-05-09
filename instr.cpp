#include <iostream>
#include <stdint.h>
#include <string>
#include <fstream>
#include "dis.h"

using namespace std;

  instru::instru(){

    opcode = 0;
    operand1 = operand2 = 0;
    oneOp = false;

    operand1r = operand2r = 0;

    operandSize = 0;

    swapOperands = 0;

    mne = reg = "";

    rex = rexW = rexR = rexX = rexB = 0;

    opPre1 = opPre2 = 0;

    modRegRM = 0;

    noOps = 0;
  }

  bool instru::getInstru(istream &file, int readPos){
    //Getting instruction 2
    pos = readPos;

    if (parsePrefix(file, pos)){
      //Increment pos if a prefix was read
      pos++;
      //Increment pos is a second prefix was read
      if(opPre2)
        pos++;
    }

    //Next byte should be the opcode, return if 0 is read
    if (!(opcode = getByte(file, pos++)))
      return false;

    if(!parseOpcode(opcode, file)){
      cout << "Read invalid opcode: " << hex << +opcode
           << " at position " << +pos << endl;
      return false;
    }

    //Instruction has no operands
    if(noOps){
      cout << "\t" << mne << endl;
      pos = file.tellg();
      return true;
    }

    //Instruction has only 1 operand
    if (oneOp){
      cout << "\t" << mne << "\t";

      if (operand2r)
        cout << getReg(operand2);
      else
        cout << "$0x" << hex << +operand2;

      cout << endl;

      pos = file.tellg();
      return true;
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

  bool instru::parsePrefix(istream &file, int pos){
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
      case 0x0F:   opPre1 = prefix;
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



  bool instru::parseOpcode(uint8_t opcode, istream &file){
    //Parse prefix byte
    if (opPre1 == 0x0F){
      switch(opcode)
        case 0x05: mne = "syscall";
		   noOps = 1;
                   return true;
    }

    //Check for B* mov instruction
    if ((opcode & 0xF0) == 0xB0){
      mne = "mov";
      //if (opcode >= 0xB8)
        operand1 = opcode & 0x07;
        operand1r = 1;
	operand2 = get4Bytes(file,pos);

	swapOperands = 1;

      return 1;

    }

    switch(opcode){
      //add reg/reg
      case 0x01: mne = "add";
		 parseRegModRM(getByte(file, pos++));
		 return true;
      //push
      case 0x68: mne = "push";
                 oneOp = true;
		 operand2 = get4Bytes(file, pos);
		 pos += 4;
                 return true;
      //add imm,reg
      case 0x83: mne = "add"; 
		 swapOperands = 1;
		 parseRegModRM(getByte(file, pos++));
		 operand2r = 0;
        	 operand2 = getByte(file, pos);

		 return true;
      //mov instruction 0x89
      case 0x89: mne = "mov";
		 parseRegModRM(getByte(file, pos++));
		 return true;
      //mov instruction	0xc7
      case 0xc7: mne = "mov";
		 //Operands are 4 bytes
		 operandSize = 4;
		 //Operand1 is a register
		 //operand1r = 1;
		 //Operands are printed in the reverse order
		 //swapOperands = 1;
		 
		 parseRegModRM(getByte(file, pos++));
		 operand1r = 0;
        	 operand1 = get4Bytes(file, pos);

		 return true;
      //mul and imul
      case 0xF7: parseRegModRM(getByte(file, pos++));
                 oneOp = true;
		 if (RM_reg == 0b101)
                   mne = "imul";
                 else
                   mne = "mul";

                 return true;
      //Return false for unknown operation
      default:   mne = "UNK_OP";
		 return false;
    }
  }

  void instru::parseRegModRM(uint8_t byte){
    modRegRM = 1;

    bool d = opcode & 0x01;

    RM_mod = byte >> 6;
    RM_reg = (byte & 0x38) >> 3;
    RM_rm = byte & 0x07;

    if (RM_mod == 0b11){
     operand1r = operand2r = 1; 
     operand1 = RM_reg;
     operand2 = RM_rm;
    }
  }

  string instru::getReg(uint8_t reg){
    //Use 64-bit registers
    if(rexW){
      switch(reg){

        case 0x0: return "%rax";
        case 0x1: return "%rcx";
        case 0x2: return "%rdx";
        case 0x3: return "%rbx";
        case 0x4: return "%rsp";
        case 0x5: return "%rbp";
        case 0x6: return "%rsi";
        case 0x7: return "%rdi";

        //Return UNK for unknown register
        default:   return "UNK";
      }
    }

    //Default to 32-bit registers
    switch(reg){

      case 0x0: return "%eax";
      case 0x1: return "%ecx";
      case 0x2: return "%edx";
      case 0x3: return "%ebx";
      case 0x4: return "%esp";
      case 0x5: return "%ebp";
      case 0x6: return "%esi";
      case 0x7: return "%edi";

      //Return UNK for unknown register
      default:   return "UNK";
    }
  }


  void instru::clearInstru(){
    opcode = 0;
    operand1 = operand2 = 0;

    operand1r = operand2r = 0;

    oneOp = 0;

    operandSize = 0;

    swapOperands = 0;

    mne = reg = "notLoaded";

    rex = rexW = rexR = rexX = rexB = 0;

    opPre1 = opPre2 = 0;

    modRegRM = 0;

    RM_mod = RM_reg = RM_rm = 0;

    noOps = 0;
  }
