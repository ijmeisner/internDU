// RSA encryption and decryption with the gmp library
// Isaac Meisner
// 6/25/15

#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <gmpxx.h>

void RSA_generateKeys(mpz_class& e, mpz_class& n, mpz_class& d);
mpz_class randomPrime();
mpz_class randomSafePrime();
mpz_class getE(mpz_class phiN);
mpz_class getD(mpz_class e, mpz_class phiN);
mpz_class RSA_encrypt(mpz_class originalMessage, mpz_class e, mpz_class n);
mpz_class RSA_decrypt(mpz_class c, mpz_class d, mpz_class n);

using namespace std;

#define MOD_BITSTRENGTH 2048
#define PRIME_STRENGTH (MOD_BITSTRENGTH/2)			//Size of p and q in bits

//Main Program
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main()
{
  cout << "----------------------------------------------------------" << endl;
  //srand((unsigned)time(0));
  srand(1234);
  mpz_class e, n, d, m1;
  RSA_generateKeys(e,n,d);
  mpz_class cipherText, message;
  ifstream infile("data.txt", ios::in);
  if(infile.is_open()){
    while(infile >> m1){
      cipherText = RSA_encrypt(m1, e, n);
      //cout << setw(19) << "Cipher Text: " << cipherText << endl << endl;
      message = RSA_decrypt(cipherText, d, n);
      //cout << setw(19) << "Decrypted Message: " << message << endl << endl;
      cout << ((m1 == message)?("WORKS!"):("doesnt work........")) << endl;
      //cout << "----------------------------------------------------------" << endl;
    }
  }

  return 0;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//
// Function that generates keys e, n, and d for RSA Encryption
//
void RSA_generateKeys(mpz_class& e, mpz_class& n, mpz_class& d)
{
  mpz_class p1 = randomPrime();
  //cout << setw(19) << "1st Random Prime: " << p1 << endl << endl;
  mpz_class p2 = randomPrime();
  //cout << setw(19) << "2nd Random Prime: " << p2 << endl << endl;
  n = p1 * p2;
  cout << setw(19) << MOD_BITSTRENGTH << " bit n = " << n << endl << endl;
  mpz_class phiN = ((p1-1)*(p2-1));
  cout << setw(19) << "phiN = " << phiN << endl << endl;
  e = getE(phiN); 
  cout << setw(19) << "e = " << e << endl << endl;
  d = getD(e, phiN);
  cout << setw(19) << "d = " << d << endl << endl;
}

//
// Function that generates a random prime of size BIT_STRENGTH
//
mpz_class randomPrime()
{
  mpz_class randNum, prime, safe;
  gmp_randclass rr(gmp_randinit_default);
  rr.seed(rand());
  randNum = rr.get_z_bits(PRIME_STRENGTH);
  mpz_nextprime(prime.get_mpz_t(), randNum.get_mpz_t());
  return prime;
}

//
// Function that generates a random safe prime of size BIT_STRENGTH
//
mpz_class randomSafePrime()
{
  mpz_class randNum, safePrime, safe;
  int reps = 25;
  int result = 0;
  while(result == 0){
    gmp_randclass rr(gmp_randinit_default);
    rr.seed(rand());
    randNum = rr.get_z_bits(PRIME_STRENGTH);

    mpz_nextprime(safePrime.get_mpz_t(), randNum.get_mpz_t());
    safe = (safePrime-1)/2;
    result = mpz_probab_prime_p(safe.get_mpz_t(), reps);
    cout << "Result: " << result << endl;
  }
  return safePrime;
}

//
// Function that calculates the GCD of e and phi(n) to determine
// the public key e
//
mpz_class getE(mpz_class phiN)
{
  //GCD of (e, phin)
  mpz_class result = 0;
  mpz_class e;
  while(result != 1){
    e = 65537;
    mpz_gcd(result.get_mpz_t(), e.get_mpz_t(), phiN.get_mpz_t());
    e += 2;
  }
  return e;
}

//
// Function that calcutates the private key d, the modular inverse
//
mpz_class getD(mpz_class e, mpz_class phiN)
{
  mpz_class d;
  mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phiN.get_mpz_t());
  return d;
}

//
// Function that encrypts a message with the public keys, e and n
// into cipher text that can be sent
//
mpz_class RSA_encrypt(mpz_class originalMessage, mpz_class e, mpz_class n)
{
  mpz_class cipherText;
  //cout << setw(19) << "Original Message: " << originalMessage << endl << endl;
  mpz_powm(cipherText.get_mpz_t(), originalMessage.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
  return cipherText; //(m^e) % n;
}

//
// Function that decrypts a cipher text with the private key d and
// the public key n
//
mpz_class RSA_decrypt(mpz_class cipherText, mpz_class d, mpz_class n)
{
  mpz_class message;
  mpz_powm(message.get_mpz_t(), cipherText.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
  return message;
}
