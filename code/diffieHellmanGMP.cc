// Diffie-Hellman Protocol for exchanging secret keys with GMP lib
// Isaac Meisner
// 6/25/15

#include <iostream>
#include <cmath>
#include <iomanip>
#include <gmpxx.h>

mpz_class diffieHellman(mpz_class base, mpz_class secretNum, mpz_class prime);
void generateRandomSafePrime(mpz_class& safePrime, mpz_class& smallPrime);
mpz_class getPrimitiveRoot(mpz_class safePrime, mpz_class smallPrime);

using namespace std;

#define BIT_STRENGTH 2048
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main()
{
	cout << "------------------------------------------------------------" << endl;
	srand((unsigned)time(0));
	mpz_class safePrime, smallPrime;
	generateRandomSafePrime(safePrime, smallPrime);
	cout << setw(19) << "Safe Prime: " << safePrime << endl << endl;
	mpz_class generator = getPrimitiveRoot(safePrime, smallPrime);

	mpz_class aliNum = 9347859583985324;
	mpz_class bobNum = 4536702573253498;

	mpz_class aliCipher, bobCipher, aliShare, bobShare;

	cout << setw(19) << "Ali's Secret Num: " << aliNum << endl;
	cout << setw(19) << "Bob's Secret Num: " << bobNum  << endl;
	cout << endl;

	aliCipher = diffieHellman(generator, aliNum, safePrime);
	bobCipher = diffieHellman(generator, bobNum, safePrime);

	cout << setw(19) << "Ali's Cipher Num: " << aliCipher << endl;
	cout << setw(19) << "Bob's Cipher Num: " << bobCipher << endl;
	cout << endl;

	aliShare = diffieHellman(bobCipher, aliNum, safePrime);
	bobShare = diffieHellman(aliCipher, bobNum, safePrime);

	if(aliShare == bobShare){
		cout << setw(19) << "Shared Number: " << aliShare << endl;
	}else{
		cout << setw(19) <<"Error!!!" << endl;
	}

	cout << "------------------------------------------------------------" << endl;

	return 0;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//
// Function that preforms Diffie-Hellman steps
//
mpz_class diffieHellman(mpz_class base, mpz_class secretNum, mpz_class prime)
{
	mpz_class result;
	mpz_powm(result.get_mpz_t(), base.get_mpz_t(), secretNum.get_mpz_t(), prime.get_mpz_t());
	return result;
}

//
// Function that generates a random safe prime of size BIT_STRENGTH
//
void generateRandomSafePrime(mpz_class& safePrime, mpz_class& smallPrime)
{
  mpz_class randNum;
  int reps = 25;
  int result = 0;
  gmp_randclass rr(gmp_randinit_default);
  rr.seed(rand());
  while(result == 0){
    randNum = rr.get_z_bits(BIT_STRENGTH);

    mpz_nextprime(smallPrime.get_mpz_t(), randNum.get_mpz_t());
    safePrime = (smallPrime*2)+1;
    result = mpz_probab_prime_p(safePrime.get_mpz_t(), reps);
  }
}

//
// Function that returns lowest primitive root of safePrime
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
  for(a=2; a<safePrime; a++){
    mpz_powm(result1.get_mpz_t(), a.get_mpz_t(), rop1.get_mpz_t(), safePrime.get_mpz_t());
    mpz_powm(result2.get_mpz_t(), a.get_mpz_t(), rop2.get_mpz_t(), safePrime.get_mpz_t());
    if((result1 != 1) && (result2 != 1)){
      //cout << a << " ";
      return a;
    }
  }
}
