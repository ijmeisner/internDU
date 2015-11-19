// Generate 2048 bit safe prime with GMP Lib
// Isaac Meisner

#include <iostream>
#include <cmath>
#include <iomanip>
#include <ctime>
#include <gmpxx.h>

void generateSafePrimeDividing(mpz_t safePrime, gmp_randstate_t gmpRandState);
void generateSafePrimeMultiplying(mpz_t safePrime, gmp_randstate_t gmpRandState);

#define BIT_STRENGTH 512

using namespace std;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main(){
  gmp_randstate_t gmpRandState;
  gmp_randinit_default(gmpRandState);
  gmp_randseed_ui(gmpRandState, 1010101);
  cout << "-----------------------------------------------------------" << endl;
  clock_t d = clock();
  double dTime;
  mpz_t dTestNum;
  mpz_init(dTestNum);
  generateSafePrimeDividing(dTestNum, gmpRandState);
  dTime = (clock()-d) / (double)CLOCKS_PER_SEC;
  cout << "Large Safe Prime by division check: " << endl;
  cout << dTestNum << endl;
  cout << "Time: " << dTime << endl;
  cout << "-----------------------------------------------------------" << endl;

  clock_t m = clock();
  double mTime;
  mpz_t mTestNum;
  generateSafePrimeMultiplying(mTestNum, gmpRandState);
  mTime = (clock()-m) / (double)CLOCKS_PER_SEC;
  cout << "Large Safe Prime by multiplication check: " << endl;
  cout << mTestNum << endl;
  cout << "Time: " << mTime << endl;
  cout << "-----------------------------------------------------------" << endl;

  mpz_clear(dTestNum);
  mpz_clear(mTestNum);

  return 0;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//
// Function to generate a safe prime the size of BIT_STRENGTH by checking if
// the number 1 less and divided by 2 is prime 
//
void generateSafePrimeDividing(mpz_t safePrime, gmp_randstate_t gmpRandState)
{
  int steps = 0;
  int randBit = BIT_STRENGTH;
  mpz_t randNum, smallPrime, diff, one, two;
  mpz_init(randNum);
  mpz_init(smallPrime);
  mpz_init(diff);
  mpz_init_set_str(one, "1", 10);
  mpz_init_set_str(two, "2", 10);
  int reps = 25;
  int result = 0;
  while(result == 0){
    mpz_urandomb(randNum, gmpRandState, randBit);
    //cout << "Rand Num = " << randNum << endl;
    steps++;
    mpz_nextprime(safePrime, randNum);
    mpz_sub(diff, safePrime, one);
    mpz_fdiv_q(smallPrime, diff, two);
    result = mpz_probab_prime_p(smallPrime, reps);
  }
  cout << "Steps = " << steps << endl;
  gmp_randclear(gmpRandState);
  mpz_clear(randNum);
}

//
// Function to generate a safe prime the size of BIT_STRENGTH by checking if
// the number multiplied by 2 and add 1 is prime
//
void generateSafePrimeMultiplying(mpz_t safePrime, gmp_randstate_t gmpRandState)
{
  int steps = 0;
  int randBit = BIT_STRENGTH-2;
  mpz_t randNum, largePrime, product, one, two;
  mpz_init(randNum);
  mpz_init(largePrime);
  mpz_init(product);
  mpz_init_set_str(one, "1", 10);
  mpz_init_set_str(two, "2", 10);
  int reps = 25;
  int result = 0;
  while(result == 0){
    mpz_urandomb(randNum, gmpRandState, randBit);
    //cout << "Rand Num = " << randNum << endl;
    steps++;
    mpz_nextprime(safePrime, randNum);
    mpz_mul(product, safePrime, two);
    mpz_add(largePrime, product, one);
    result = mpz_probab_prime_p(largePrime, reps);
  }
  cout << "Steps = " << steps << endl;
  gmp_randclear(gmpRandState);
  mpz_clear(randNum);
  mpz_clear(largePrime);
}

