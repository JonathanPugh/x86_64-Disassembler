#include <stdint.h>
#include <iostream>
#include <fstream>

#include "dis.h"
//#include "instr.h"

using namespace std;

int main(int argc, char** argv){

  string fileName = "a.out";

  if (argc > 2){
    cout << "Too many parameters specified" << endl;
    cout << "Use --help for help" << endl;
    return -3;
  }

  //If a command line parameter was specified
  if (argv[1]){
    if (argv[1] == string("--help")){
      cout << "Help text not added yet =(" << endl;
      return 0;
    } else
      fileName = string(argv[1]);
  }

  ifstream inFile(fileName.c_str(), std::ios::binary);

  //Input file not found
  if (!inFile){
    cout << "File \"" << fileName << "\" not found" << endl;
    return -4;
  }
 
  //Verify that ELF header exists
  if(!verifyHeader(inFile)){
    cout << "Failed to identify ELF header" << endl;
    return -1;
  }

  //Return if architecture cannot be identified
  if(!verifyArch(inFile))
  {
    cout << "Failed to identify architecture" << endl;
    return -2;
  }

  //Print the entrypoint address
  //cout << "Entry point found at: "
  //     << hex << entryPoint(inFile) << endl;

  uint64_t currentPos = entryPoint(inFile) ;

  instru i;

  //Hard coded header
  cout << "\t.global _start\n\n\t.text\n_start:" << endl;


  while(i.getInstru(inFile, currentPos)){

    currentPos = i.pos;

    i.clearInstru();
  } 
}
