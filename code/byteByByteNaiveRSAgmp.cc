// RSA implentation with GMP Library
// Isaac Meisner
// 7/6/15

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
void RSA_encrypt(mpz_class e, mpz_class n);
void RSA_decrypt(mpz_class d, mpz_class n);

using namespace std;

#define READ_SIZE 125				//in bytes
#define BUFFER_SIZE (READ_SIZE+8)
#define BIT_STRENGTH (READ_SIZE*8)/2+1		//Size of random prime number 
#define MESSAGE "linux.JPG"
#define CIPHERTEXT "cipherfile.JPG"
#define PLAINTEXT "recoveredLinux.JPG"

//Main Program
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main()
{
  cout << 
"_____________________________________________________________________________________________________"
  << endl;
  srand(1234);
  //srand((unsigned)time(0));
  mpz_class e, n, d;
  RSA_generateKeys(e, n, d);

  RSA_encrypt(e, n);
  RSA_decrypt(d, n);
  cout <<
"_____________________________________________________________________________________________________"
  << endl;

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
  cout << setw(19) << "Public Key n = " << n << endl << endl;
  mpz_class phiN = ((p1-1)*(p2-1));
  cout << setw(19) << "Private phiN = " << phiN << endl << endl;
  e = getE(phiN); 
  cout << setw(19) << "Public Key e = " << e << endl << endl;
  d = getD(e, phiN);
  cout << setw(19) << "Private Key d = " << d << endl << endl;
}

//
// Function that generates a random prime of size BIT_STRENGTH
//
mpz_class randomPrime()
{
  mpz_class randNum, prime, safe;
  gmp_randclass rr(gmp_randinit_default);
  rr.seed(rand());
  randNum = rr.get_z_bits(BIT_STRENGTH);
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
    randNum = rr.get_z_bits(BIT_STRENGTH);

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
    e = random();
    mpz_gcd(result.get_mpz_t(), e.get_mpz_t(), phiN.get_mpz_t());
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
void RSA_encrypt(mpz_class e, mpz_class n)
{
  streampos readSize;
  int fileSize;
  char* msgblock;
  char* cipherblock;
  char* buffer;
  mpz_class c, M;
  size_t count;
  size_t* countp;
  int order = -1;
  size_t size;
  int endian = 0;
  size_t nails = 0; 
  int tmp;

  cout << "Encrypting : " << MESSAGE << endl;
  cout << "-----------------------------" << endl;

  ifstream infile (MESSAGE, ios::in|ios::binary|ios::ate);
  ofstream outfile(CIPHERTEXT, ios::out|ios::binary);
  //ofstream out(MESSAGE, ios::out|ios::binary|ios::app);
  if(infile.is_open() && outfile.is_open()){
    //readSize = READ_IN;
    readSize = READ_SIZE;
    fileSize = infile.tellg();
    //while(fileSize % readSize != 0){
      //out << ' ';
      //fileSize = out.tellp();
    //}
    //out.close();

    cout << "Read-in Size: " << readSize << " bytes / " << readSize*8 << " bits" << endl;
    cout << "File Size: " << fileSize << " bytes / " << fileSize*8 << " bits" << endl << endl;
    
    //msgblock = new char [readSize];
    //cipherblock = new char [readSize];
    buffer = new char[BUFFER_SIZE];
    for(int x=0; x<BUFFER_SIZE; x++){
      buffer[x] = '\0';
    }
    infile.seekg(0, ios::beg);

    while(infile.read(buffer, READ_SIZE)){
    //for(int i = 0; i<3; i++){
      mpz_import(M.get_mpz_t(), READ_SIZE, order, 1, endian, nails, buffer);
      cout << "M = " << M.get_mpz_t() << endl;
      mpz_powm(c.get_mpz_t(), M.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
      cout << "c = " << c.get_mpz_t() << endl;
      mpz_export(buffer, &count, order, 1, endian, nails, c.get_mpz_t());
      outfile.write(buffer, BUFFER_SIZE);
    }
    cout << endl;
  }else cout << "Unable to open file!" << endl;

  //delete [] msgblock;
  //delete [] cipherblock;
  delete [] buffer;
  infile.close();
  outfile.close();
}

//
// Function that decrypts a cipher text with the private key d and
// the public key n
//
void RSA_decrypt(mpz_class d, mpz_class n)
{
  streampos readSize;
  int fileSize;
  char* cipherblock;
  char* plainblock;
  char* buffer;
  mpz_class m, C;
  size_t count;
  size_t* countp;
  int order = -1;
  size_t size;
  int endian = 0;
  size_t nails = 0;
  char tmp;

  cout << "Decrypting : " << CIPHERTEXT << endl;
  cout << "-----------------------------" << endl;

  ifstream infile(CIPHERTEXT, ios::in|ios::binary|ios::ate);
  ofstream outfile(PLAINTEXT, ios::out|ios::binary);
  if(infile.is_open() && outfile.is_open()){
    //readSize = READ_IN;
    readSize = BUFFER_SIZE;
    fileSize = infile.tellg();
    cout << "Read-in Size: " << readSize << " bytes / " << readSize*8 << " bits" << endl;
    cout << "File Size: " << fileSize << " bytes / " << fileSize*8 << " bits" << endl << endl;

    //cipherblock = new char[readSize];
    //plainblock = new char[readSize];
    buffer = new char[BUFFER_SIZE];
    for(int x=0; x<BUFFER_SIZE; x++){
      buffer[x] = '\0';
    }

    infile.seekg (0, ios::beg);
    while(infile.read(buffer, BUFFER_SIZE)){
    //for(int i=0; i<3; i++){
      mpz_import(C.get_mpz_t(), BUFFER_SIZE, order, 1, endian, nails, buffer);
      mpz_powm(m.get_mpz_t(), C.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
      cout << "m = " << m.get_mpz_t() << endl;
      cout << "C = " << C.get_mpz_t() << endl;
      mpz_export(buffer, &count, order, 1, endian, nails, m.get_mpz_t());
      outfile.write(buffer, READ_SIZE);
    }
    cout << endl;
  }else cout << "Unable to open file!" << endl;
  cout << "Creating : " << PLAINTEXT << endl;
  cout << "-----------------------------" << endl;

  //delete [] cipherblock;
  //delete [] plainblock;
  delete [] buffer;
  infile.close();
  outfile.close();
}

