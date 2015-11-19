// Generate 2048 bit safe prime with GMP Lib
// Isaac Meisner
// 6/30/15

#include <iostream>
#include <cmath>
#include <iomanip>
#include <ctime>
#include <gmpxx.h>

mpz_class generateSafePrimeDividing();
mpz_class generateSafePrimeMultiplying();
int isSafePrime(mpz_class x);

#define BIT_STRENGTH 1024

using namespace std;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main(){
  //srand((unsigned)time(0));
  srand(1234);

  cout << "-----------------------------------------------------------" << endl;
  clock_t d = clock();
  double dTime;
  mpz_class dTestNum;
  dTestNum = generateSafePrimeDividing();
  dTime = (clock()-d) / (double)CLOCKS_PER_SEC;
  cout << "Large Safe Prime by division check: " << endl;
  cout << dTestNum << endl;
  cout << "Time: " << dTime << endl;
  cout << "-----------------------------------------------------------" << endl;

  clock_t m = clock();
  double mTime;
  mpz_class mTestNum;
  mTestNum = generateSafePrimeMultiplying();
  mTime = (clock()-m) / (double)CLOCKS_PER_SEC;
  cout << "Large Safe Prime by multiplication check: " << endl;
  cout << mTestNum << endl;
  cout << "Time: " << mTime << endl;
  cout << "-----------------------------------------------------------" << endl;

 /*mpz_class x = 961748941;//858599509;
  int result;
  result = isSafePrime(x);
  if(result == 2){
    cout << "Definitely Safe Prime!" << endl;
  }
  if(result == 1){
    cout << "Probably safe prime" << endl;
  }
  if(result == 0){
    cout << "Definitely not safe prime" << endl;
  }
*/

  return 0;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//
// Function to generate a safe prime the size of BIT_STRENGTH by checking if
// the number 1 less and divided by 2 is prime 
//
mpz_class generateSafePrimeDividing()
{
  int steps = 0;
  mpz_class randNum, smallPrime, safePrime, rop;
  int reps = 25;
  int result = 0;
  gmp_randclass rr(gmp_randinit_default);
  rr.seed(9);
  while(result == 0){
    randNum = rr.get_z_bits(BIT_STRENGTH);
    //cout << "Rand Num = " << randNum << endl;
    steps++;

    mpz_nextprime(safePrime.get_mpz_t(), randNum.get_mpz_t());
    smallPrime = (safePrime-1)/2;
    result = mpz_probab_prime_p(smallPrime.get_mpz_t(), reps);
  }
  cout << "Steps = " << steps << endl;
  return safePrime;
}

//
// Function to generate a safe prime the size of BIT_STRENGTH by checking if
// the number multiplied by 2 and add 1 is prime
//
mpz_class generateSafePrimeMultiplying()
{
  int steps = 0;
  mpz_class randNum, smallPrime, safePrime;
  int reps = 25;
  int result = 0;
  gmp_randclass rr(gmp_randinit_default);
  rr.seed(9);
  while(result == 0){
    randNum = rr.get_z_bits(BIT_STRENGTH - 2);
    steps++;

    mpz_nextprime(smallPrime.get_mpz_t(), randNum.get_mpz_t());
    safePrime = (smallPrime*2)+1;
    result = mpz_probab_prime_p(safePrime.get_mpz_t(), reps);
  }
  cout << "Steps = " << steps << endl;
  return safePrime;
}

//
// Function to check if a number is a safe prime
//
int isSafePrime(mpz_class x)
{
  mpz_class safe;
  safe = (x-1)/2;
  
  int reps = 25;
  int result;

  result = mpz_probab_prime_p(safe.get_mpz_t(), reps);

  return result;
}
