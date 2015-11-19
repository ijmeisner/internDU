#include <iostream>
#include <fstream>
using namespace std;

void decrypt(char* memblock, int size);

int main(){
  streampos size;
  int fileSize;
  char* memblock;
 
  //Decryption
  ifstream infile ("cipherText.txt", ios::in|ios::binary|ios::ate);
  ofstream outfile("plainText.txt", ios::out|ios::binary);
  if(infile.is_open()){
    fileSize = infile.tellg();
    size = 8;
    cout << "Read-in Size: " << size << " bits / " << size/8 << " byte(s)" << endl; 
    cout << "File Size: " << fileSize << " bits / " << fileSize/8 << " byte(s)" << endl; 
    memblock = new char [size];
    infile.seekg (0, ios::beg);
    for(int i=0; i<fileSize; i+=size){
      infile.read(memblock, size);
      decrypt(memblock, size);
      for(int j = 0; j<size; j++){
        outfile << memblock[j];
        cout << memblock[j];
      }
    }
    infile.close();
    outfile.close();
  }else cout << "Unable to open file!" << endl;  

  delete[] memblock;

  return 0;
}

void decrypt(char* memblock, int size)
{
  for(int i = 0; i<size; i+=8){
    memblock[i  ] -= 65;  //A
    memblock[i+1] -= 77;  //M
    memblock[i+2] -= 69;  //E
    memblock[i+3] -= 82;  //R
    memblock[i+4] -= 73;  //I
    memblock[i+5] -= 67;  //C
    memblock[i+6] -= 65;  //A
    memblock[i+7] -= 33;  //!
  }
}
