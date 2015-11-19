#include <iostream>
#include <fstream>
using namespace std;

int main () {
  streampos size;
  char * memblock;

  ifstream file ("example.txt", ios::in|ios::binary|ios::ate);
  if (file.is_open())
  {
    size = file.tellg();
    memblock = new char [size];
    file.seekg (0, ios::beg);
    file.read (memblock, size);
    file.close();

    cout << "the entire file content is in memory" << endl;

    delete[] memblock;
  }
  else cout << "Unable to open file";
  cout << endl;

  string result(memblock);
  cout << result << endl;    
  return 0;
}
