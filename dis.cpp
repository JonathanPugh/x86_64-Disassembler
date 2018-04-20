#include <stdint.h>
#include <iostream>
#include <fstream>
#include "dis.h"

using namespace std;

int main(){

  ifstream inFile("a.out", std::ios::binary);
 
  //Verify that ELF header exists
  if(verifyHeader(inFile)){
    cout << "Identified ELF header" << endl;
  } else {
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
  cout << "Entry point found at: "
       << hex << entryPoint(inFile) << endl;

  //Read first byte from text segment
  cout << "First byte: " << +getByte(inFile, entryPoint(inFile)) << endl;

}
