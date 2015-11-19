// Isaac Meisner
// 7/6/15

#include <iostream>
#include <fstream>
using namespace std;

void encrypt(char* memblock, int size);

int main(){
  streampos size;
  int fileSize;
  char* memblock;

  ifstream infile ("message.txt", ios::in|ios::binary|ios::ate);
  ofstream outfile("cipherText.txt", ios::out|ios::binary);
  if(infile.is_open()){
    size = 8;
    fileSize = infile.tellg();
    cout << "Read-in Size: " << size << " bits / " << size/8 << " byte(s)" << endl;
    cout << "File Size: " << fileSize << " bits / " << fileSize/8 << " byte(s)" << endl;
    if(fileSize % 8 != 0){
      cout << "File is not correct size!" << endl;
      return 0;
    }
    memblock = new char [size];
    infile.seekg (0, ios::beg);
    for(int i=0; i<fileSize; i+=size){
      infile.read(memblock, size);
      encrypt(memblock, (int)size);
      if(outfile.is_open()){
        for(int j = 0; j<size; j++){
          outfile << memblock[j];
          cout << memblock[j];
        }
      }
    }
  }else cout << "Unable to open file!" << endl;  
  
  cout << endl;

  delete[] memblock;
  infile.close();
  outfile.close();
  
  return 0;
}


void encrypt(char* memblock, int size)
{
 for(int i = 0; i<size; i+=8){
    memblock[i  ] += 65;  //A
    memblock[i+1] += 77;  //M
    memblock[i+2] += 69;  //E
    memblock[i+3] += 82;  //R
    memblock[i+4] += 73;  //I
    memblock[i+5] += 67;  //C
    memblock[i+6] += 65;  //A
    memblock[i+7] += 33;  //!
  }
}

