#include <iostream>
#include <fstream>
#include <gmpxx.h>

using namespace std;

#define READ_IN 25
#define IN_FILE "linux.JPG"
#define OUT_FILE "copylinux.JPG"

int main(){

  cout << "______________________________________________________________________" << endl;
  
  streampos readSize;
  int fileSize;
  char* memblock;
  char* buffer;
  mpz_class rop;
  mpz_class tmp;
  size_t count;
  int order = 1;
  size_t size;
  int endian = 0;
  size_t nails = 0;
  size_t* countp;

  ifstream infile(IN_FILE, ios::in|ios::binary|ios::ate);
  ofstream outfile(OUT_FILE, ios::out|ios::binary);
  if(infile.is_open() && outfile.is_open()){
    fileSize = infile.tellg();
    //readSize = fileSize/11;
    readSize = READ_IN;

    cout << "Read-in Size: " << readSize << " bytes / " << readSize*8 << " bits" << endl;
    cout << "File Size: " << fileSize << " bytes / " << fileSize*8 << " bits" << endl << endl; 

    memblock = new char[readSize];
    buffer = new char[readSize];
    infile.seekg(0, ios::beg);

    while(infile.read(memblock, readSize)){
    //for(int i=0; i<30; i++){
      //infile.read(memblock, readSize);
      mpz_import(rop.get_mpz_t(), readSize, order, sizeof(memblock[0]), endian, nails, memblock);
      //cout << rop << endl << endl;
      

      mpz_export(buffer, countp, order, sizeof(memblock[0]), endian, nails, rop.get_mpz_t());
      outfile.write(buffer, readSize);
    }
  }

  cout << endl;
  cout << "______________________________________________________________________" << endl;

  delete [] memblock;
  delete [] buffer;
  infile.close();
  outfile.close();

  return 0;
}
