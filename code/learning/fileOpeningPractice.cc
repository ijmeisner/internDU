#include <iostream>
#include <fstream>

using namespace std;

int main(){
  ifstream infile("example.txt", ios::binary | ios::in);
  //ofstream outfile("newExample.txt", ios::binary);
  char x;

  if(infile.is_open() /*&& outfile.is_open()*/){
    while(!infile.eof()){
      //outfile.put(infile.get());
      infile >> x; 
      cout << x << " ";
    }
    cout << endl;
  }

  infile.close();
  //outfile.close();

  return 0;
}
