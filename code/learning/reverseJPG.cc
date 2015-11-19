#include <iostream>
#include <fstream>

using namespace std;

int main(){

  char *buffer;
  ifstream infile("puppy.jpg",ios::binary|ios::in|ios::ate);
  int size = infile.tellg();
  cout << size << endl;
  buffer = new char[size];
  infile.seekg(0, ios::beg);
  infile.read(buffer, size); 
  infile.close();

  char *temp = new char[size];
  for(int i=0; i<size-1; i++){
    temp[i]=buffer[(size-2)-i];
  } 
  temp[size-1] = buffer[size-1]; //eof character 

  ofstream os("revpuppy.jpg",ios::binary); 
  os.write(temp, size); 
  os.close();

  return 0;
}
