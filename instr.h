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
  int64_t oper1, oper2;

  string mne, reg;
  bool twoOps;

  //REX prefix variables
  bool rex, rexW, rexR, rexX, rexB;

  //Variable length opcode prefixes
  bool opPre1, opPre2;

  instru(){

    opcode = 0;
    oper1 = oper2 = 0;
    twoOps = false;

    rex = rexW = rexR = rexX = rexB = 0;

    opPre1 = opPre2 = 0;
  }

  bool getInstru(istream &file, int readPos){
    pos = readPos;
    opcode = getByte(file, pos);

    if (parsePrefix(file, readPos)){
      //Increment pos if a prefix was read
      pos++;
      //Increment pos is a second prefix was read
      if(opPre2)
        pos++;
    }

    //End function early for debugging
    return 1;

    //Identify REX byte
    if (opcode == 0x48){
      rex = 1;
      //cout << "Found the REX" << endl;
      opcode = getByte(file, ++pos);
    }

    mne = getMne();

    switch(opcode){
 
      //mov instruction	
      case 0xc7: twoOps = true;
                 break;

      //SYSCALL - Skip next byte, end method
      case 0x0f: pos += 2;
		 return true;
		 break;

      case 0x00: //cout << "Found 0x00 opcode" << endl;
		 return false;
		 break;

      //Return false for unknown operation
      default:   return false;

    }

    oper1 = getByte(file, ++pos);



    if(twoOps)
      if(rex)
        oper2 = get4Bytes(file, ++pos);
      else
        oper2 = getByte(file, ++pos);

    pos = file.tellg();

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
      case 0xF0: {
		   opPre1 = prefix;
		   int prefix2 = getByte(file, pos + 1);

                   if (prefix2 == 0x38 || prefix2 == 0x3A)
                     opPre2 = prefix2;
 
                   return true;
                 }
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



  string getMne(){
    switch(opcode){
      //mov instruction	
      case 0xc7: return "mov";

      //SYSCALL
      case 0x0f: return "syscall";

      //Return UNK for unknown operation
      default:   return "UNK";
    }
  }


  string getReg(int64_t reg){

    switch(reg){
      //%rax
      case 0xc0: return "%rax";

      //Return UNK for unknown register
      default:   return "UNK";
    }
  }


  void clearInstru(){
    oper1 = 0; 
    oper2 = 0;
    rex = twoOps = false;
  }
};

#endif
