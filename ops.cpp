#include <stdint.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "dis.h"

using namespace std;

//Used by getByte to check if a position was entered
const int DEFAULTPOS = -1;

/*****************************************************
 *  Retrieves one byte from the specified file.
 *
 *  @param file - The file to read from
 *  @param pos  - The position in the file to read from.
 *  		  Reads from current position if unspecified.
 *  @return an 8 bit integer that stores the retrieved data.
 *
 * **************************************************/
uint8_t getByte(istream& file, int pos = DEFAULTPOS){

  //Seek to position if it was specified
  if (pos != DEFAULTPOS)
    file.seekg(pos);

  char * c = new char[1];

  file.read(c, 1);

  uint8_t val = c[0];

  return val;
}

/*****************************************************
 *  Verifies that the ELF header exists in the file.
 *
 *  @param file - The file to read from
 *
 *  @return true if the header is verified, false otherwise
 *
 * **************************************************/
bool verifyHeader(istream& file){

  //Start at the beginning of the file
  file.seekg(0);

  //First 4 bytes of a correct ELF header
  uint8_t elfArr[] = { 0x7f, 0x45, 0x4c, 0x46 };

  //Create a vector using data from elfArr[]
  vector<uint8_t> elfVect(elfArr, elfArr + sizeof(elfArr)/sizeof(uint8_t));

  //Create vector to store bytes read from file
  vector<uint8_t> headVect;

  //Read 4 bytes from file
  for (int i = 0; i < 4; i++)
    headVect.push_back(getByte(file, i));

  //Verify the read bytes match a correct ELF header
  if (elfVect == headVect)
    return true;
  else
    return false;
}

/*****************************************************
 *  Verifies the architecture byte is valid in the ELF
 *  header and prints the identified architecture.
 *
 *  @param file - The file to read from
 *
 *  @return true if the architecture was identified
 *  successfully, false otherwise.
 *
 * **************************************************/
bool verifyArch(istream& file){

  uint8_t arch = getByte(file, 4);

  if (arch == 1){
    cout << "Identified 32-bit" << endl;
    return true;
  }
  if (arch == 2){
    cout << "Identified 64-bit" << endl;
    return true;
  }

  return false;
}

/*****************************************************
 *  Finds the entrypoint for the text segment in 
 *  the ELF header
 *
 *  @param file - The file to read from
 *
 *  @return the private memory address of the 
 *  	    beginning of the text segment
 *
 * **************************************************/
long int entryPoint (istream &file){

  //8 bytes required for memory address
  long int addr = 0,
           tempAddr = 0;

  for (int i = 0; i < 8; i++){
    tempAddr = getByte(file, ENTRYPOINT + i);

    uint8_t upperVal, lowerVal;

    //Store lower 4 bits
    lowerVal = tempAddr & 0x0f;

    //Store upper 4 bits
    upperVal = tempAddr & 0xf0;

    //Shift upperVal right by 4 bits
    upperVal = upperVal >> 4;

    //Shift lowerVal left by 4 bits
    upperVal = upperVal << 4;

    //Add address bytes in the correct order
    tempAddr = upperVal + lowerVal;

    //Store address bytes at the correct power
    tempAddr = tempAddr << i * 8;

    //Add corrected byte to addr
    addr += tempAddr;
  }

  return addr - 0x400000;

}

