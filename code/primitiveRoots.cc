// Functions to find lowest and random  primitive roots of a safe prime
// Isaac Meisner
// 7/1/15

#include <iostream>
#include <cmath>
#include <iomanip>
#include <gmpxx.h>

mpz_class getPrimitiveRoot(mpz_class safePrime, mpz_class smallPrime);
mpz_class getRandomPrimitiveRoot(mpz_class safePrime, mpz_class smallPrime);
void generateSafePrimeMultiplying(mpz_class& safePrime, mpz_class& smallPrime);

using namespace std;

#define BIT_STRENGTH 1024

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main()
{
  srand((unsigned)time(0));
  mpz_class lowPrimitiveRoot, randPrimitiveRoot, safePrime, smallPrime;
  generateSafePrimeMultiplying(safePrime, smallPrime);
  cout << "Safe Prime = " << safePrime << endl;
  cout << "Small Prime = " << smallPrime << endl;
  lowPrimitiveRoot = getPrimitiveRoot(safePrime, smallPrime);
  randPrimitiveRoot = getRandomPrimitiveRoot(safePrime, smallPrime);
  cout << "Lowest primitive root of " << safePrime << " is "  << lowPrimitiveRoot << endl; 
  cout << "Random primitive root of " << safePrime << " is "  << randPrimitiveRoot << endl; 

  return 0;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//
// Function to generate a safe prime the size of BIT_STRENGTH and 
// the prime factor other than 2 that helps with primitive root calculation
//
void generateSafePrimeMultiplying(mpz_class& safePrime, mpz_class& smallPrime)
{
  mpz_class randNum;
  int reps = 25;
  int result = 0;
  while(result == 0){
    gmp_randclass rr(gmp_randinit_default);
    rr.seed(rand());
    randNum = rr.get_z_bits(BIT_STRENGTH);

    mpz_nextprime(smallPrime.get_mpz_t(), randNum.get_mpz_t());
    safePrime = (smallPrime*2)+1;
    result = mpz_probab_prime_p(safePrime.get_mpz_t(), reps);
  }
}

//
// Funciton to generate lowest primitive root of a safe prime
// or generate all primitive roots of a safe prime
//
mpz_class getPrimitiveRoot(mpz_class safePrime, mpz_class smallPrime)
{
  mpz_class a, phi, factor1, factor2, rop1, rop2, result1, result2;
  phi = safePrime-1;
  factor1 = 2;
  factor2 = smallPrime;
  rop1 = phi/factor1;
  rop2 = phi/factor2;
  //cout << "Primitive roots of "<< safePrime << " are ";
  for(a=2; a<30; a++){
    mpz_powm(result1.get_mpz_t(), a.get_mpz_t(), rop1.get_mpz_t(), safePrime.get_mpz_t()); 
    mpz_powm(result2.get_mpz_t(), a.get_mpz_t(), rop2.get_mpz_t(), safePrime.get_mpz_t()); 
    if((result1 != 1) && (result2 != 1)){
      //cout << a << " ";
      return a;
    }
  }
  //cout << endl;
  //return a;
}

//
// Function to generate random primitive root of a safe prime 
//  
mpz_class getRandomPrimitiveRoot(mpz_class safePrime, mpz_class smallPrime)
{
  mpz_class a, phi, factor1, factor2, rop1, rop2, result1, result2, randomNum;
  phi = safePrime-1;
  factor1 = 2;
  factor2 = smallPrime;
  rop1 = phi/factor1;
  rop2 = phi/factor2;
  result1 = 0;
  result2 = 0;
  while((result1 != 1) && (result2 != 1)){
    randomNum = rand();
    mpz_mod(a.get_mpz_t(), randomNum.get_mpz_t(), safePrime.get_mpz_t());
    mpz_powm(result1.get_mpz_t(), a.get_mpz_t(), rop1.get_mpz_t(), safePrime.get_mpz_t()); 
    mpz_powm(result2.get_mpz_t(), a.get_mpz_t(), rop2.get_mpz_t(), safePrime.get_mpz_t()); 
  }
  return a;
}
