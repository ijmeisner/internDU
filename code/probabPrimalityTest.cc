#include <iostream>
#include <cmath>
#include <iomanip>
#include <gmpxx.h>

using namespace std;

int main(){

  mpz_class a = 15485863;
  int reps = 100;
  int result;

  result = mpz_probab_prime_p(a.get_mpz_t(), reps);
  
  if(result == 2){
    cout << "Def Prime!" << endl;
  }
  if(result == 1){
    cout << "Prob prime" << endl;
  }
  if(result == 0){
    cout << "Def Composite" << endl;
  }

  cout << result << endl;

  return 0;
}

