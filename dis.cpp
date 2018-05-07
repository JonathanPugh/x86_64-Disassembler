#include <stdint.h>
#include <iostream>
#include <fstream>

#include "dis.h"
#include "instr.h"

using namespace std;

int main(){

  ifstream inFile("a.out", std::ios::binary);
 
  //Verify that ELF header exists
  if(!verifyHeader(inFile)){
    cout << "Failed to identify ELF header" << endl;
    return -1;
  }

  //Return if architecture cannot be identified
  //verifyArch() will print the identified architecture
  if(!verifyArch(inFile))
  {
    cout << "Failed to identify architecture" << endl;
    return -2;
  }

  //Print the entrypoint address
  //cout << "Entry point found at: "
  //     << hex << entryPoint(inFile) << endl;

  uint8_t currentPos = entryPoint(inFile) ;

  instru i;

  //i.getInstru(inFile, currentPos);

  //Hard coded header
  //cout << "\t.global _start\n\n\t.text\n_start:" << endl;


  while(i.getInstru(inFile, currentPos)){

    currentPos = i.pos;

    i.clearInstru();
  } 
    //cout << hex << +getByte(inFile, currentPos) << endl;
    //cout << +i.opcode << endl;


}
