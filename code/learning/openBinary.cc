#include <iostream>
#include <fstream>
using namespace std;

void encrypt(char* memblock, int size);
void decrypt(char* memblock, int size);

int main(){
  streampos size;
  int fileSize;
  char* memblock;
  char* memblock2;

  //Encryption
  ifstream infile ("message.txt", ios::in|ios::binary|ios::ate);
  ofstream outfile("cipherText.txt", ios::out|ios::binary);
  if(infile.is_open()){
    fileSize = infile.tellg();
    size = 8;
    cout << "Size: " << size << endl; 
    memblock = new char [size];
    infile.seekg (0, ios::beg);
    for(int i=0; i<fileSize; i+=size){
      infile.read(memblock, size);
      encrypt(memblock, (int)size);
      //file.seekg (i+8);
      for(int j = 0; j<size; j++){
        outfile << memblock[j];
        cout << memblock[j];
      }
    }
    infile.close();
  }else cout << "Unable to open file!" << endl;  

  delete[] memblock;
  

  //Decryption
  ifstream in ("cipherText.txt", ios::in|ios::binary|ios::ate);
  ofstream out("plainText.txt", ios::out|ios::binary);
  if(in.is_open()){
    fileSize = in.tellg();
    size = 8;
    cout << "Size: " << size << endl; 
    memblock2 = new char [size];
    in.seekg (0, ios::beg);
    for(int i=0; i<fileSize; i+=size){
      in.read(memblock2, size);
      decrypt(memblock2, size);
      for(int j = 0; j<size; j++){
        out << memblock2[j];
        cout << memblock2[j];
      }
    }
    in.close();
  }else cout << "Unable to open file!" << endl;  

  delete[] memblock;

  cout << endl;

  return 0;
}


void encrypt(char* memblock, int size)
{
  for(int i = 0; i<size; i++){
    memblock[i] += 1;
  }
}

void decrypt(char* memblock, int size)
{
  for(int i = 0; i<size; i++){
    memblock[i] -= 1;
  }
}
