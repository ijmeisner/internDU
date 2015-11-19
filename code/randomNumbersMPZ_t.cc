#include <iostream>
#include <gmpxx.h>

using namespace std;

int main(){
  int i;
  mpz_t randNum;
  mpz_t randBnd;
  int randBit;
  gmp_randstate_t gmpRandState;

  mpz_init(randNum);
  randBit = 2048;
  mpz_init_set_str(randBnd, "1000", 10);
  
  gmp_randinit_default(gmpRandState);
  gmp_randseed_ui(gmpRandState, 1234567890);

  for(i=0; i<10; i++) {
    /* mpz_get_ui() can convert randNum to unsigned long int... */
    mpz_urandomb(randNum, gmpRandState, randBit);
    //mpz_out_str(stdout, 10, randNum);
    cout << randNum << endl << endl << endl;
  }

  gmp_randclear(gmpRandState);

  mpz_clear(randNum);
  mpz_clear(randBnd);

  return 0;
}


