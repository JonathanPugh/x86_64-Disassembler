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
  bool rex, twoOps;


  instru(){

    opcode = 0;
    oper1 = oper2 = 0;
    rex = twoOps = false;

  }

  bool getInstru(istream &file, int readPos){
    pos = readPos;
    opcode = getByte(file, pos);

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
