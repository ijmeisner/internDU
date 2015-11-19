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

#define BUFFER_SIZE ((PRIME_STRENGTH*2)/8)		//Size of buffer in bytes
//#define READ_SIZE (BUFFER_SIZE)
#define READ_SIZE (BUFFER_SIZE-1)
#define PRIME_STRENGTH 1024				//Size of random prime number in bits 
#define MESSAGE "linux.JPG"	//"longTxt.txt"
#define CIPHERFILE "cipherfile.JPG"
#define PLAINFILE "plainfile.JPG"

//Main Program
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
// Function that generates a random prime of size PRIME_STRENGTH
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
// Function that generates a random safe prime of size PRIME_STRENGTH
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
  mpz_class e = 3; //65537 ??
  while(result != 1){
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
void RSA_encrypt(mpz_class e, mpz_class n)
{
  int fileSize;
  char* buffer;
  char* zero;
  mpz_class c, M;
  size_t count;
  int order = -1;
  int endian = 0;
  size_t nails = 0; 
  size_t csize;

  cout << "Encrypting : " << MESSAGE << endl;
  cout << "-----------------------------" << endl;

  ifstream infile (MESSAGE, ios::in|ios::binary|ios::ate);
  ofstream outfile(CIPHERFILE, ios::out|ios::binary);
  if(infile.is_open() && outfile.is_open()){
    fileSize = infile.tellg();

    cout << "Read-in Size: " << READ_SIZE << " bytes / " << READ_SIZE*8 << " bits" << endl;
    cout << "Buffer Size: " << BUFFER_SIZE << " bytes / " << BUFFER_SIZE*8 << " bits" << endl;
    cout << "File Size: " << fileSize << " bytes / " << fileSize*8 << " bits" << endl << endl;
    
    buffer = new char[BUFFER_SIZE];
    zero = new char[BUFFER_SIZE];
    //zero = new char[1];
    zero[0] = 0;
    memset(buffer, 0, BUFFER_SIZE);
    infile.seekg(0, ios::beg);

    while(infile.read(buffer, READ_SIZE)){
    //while(!infile.eof()){
      //infile.read(buffer, READ_SIZE);
      mpz_import(M.get_mpz_t(), READ_SIZE, order, 1, endian, nails, buffer);
      //cout <<"msize = " << mpz_sizeinbase(M.get_mpz_t(), 2) << endl;
      //cout << "m = " << M.get_mpz_t() << endl;
      mpz_powm(c.get_mpz_t(), M.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
      csize = mpz_sizeinbase(c.get_mpz_t(), 2);
      //cout <<"csize = " << mpz_sizeinbase(c.get_mpz_t(), 2) << endl;
      //cout << "c = " << c.get_mpz_t() << endl;
      mpz_export(buffer, &count, order, 1, endian, nails, c.get_mpz_t());
      //cout << "count = " << count << endl;
      if(count < BUFFER_SIZE){
        //for(int i=0; i<BUFFER_SIZE-count; i++){
          //outfile << zero;
        //}
        //outfile.write(buffer, count);
        strcat(zero, buffer);
        outfile.write(zero, BUFFER_SIZE);
      }else if(count > BUFFER_SIZE){
        cout << "ERROR!" << endl;
      }else{
	outfile.write(buffer, BUFFER_SIZE);
      }
      memset(buffer, 0, BUFFER_SIZE);
    }
    cout << endl;
  }else cout << "Unable to open file!" << endl;

  delete [] zero;
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
  int fileSize;
  char* buffer;
  mpz_class m, C;
  size_t count;
  int order = -1;
  int endian = 0;
  size_t nails = 0;

  cout << "Decrypting : " << CIPHERFILE << endl;
  cout << "-----------------------------" << endl;

  ifstream infile(CIPHERFILE, ios::in|ios::binary|ios::ate);
  ofstream outfile(PLAINFILE, ios::out|ios::binary);
  if(infile.is_open() && outfile.is_open()){
    fileSize = infile.tellg();
    cout << "Read-in/buffer Size: " << BUFFER_SIZE << " bytes / " << BUFFER_SIZE*8 << " bits" << endl;
    cout << "File Size: " << fileSize << " bytes / " << fileSize*8 << " bits" << endl << endl;

    buffer = new char[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    infile.seekg (0, ios::beg);

    while(infile.read(buffer, BUFFER_SIZE)){
    //while(!infile.eof()){
      //infile.read(buffer, BUFFER_SIZE);
      mpz_import(C.get_mpz_t(), BUFFER_SIZE, order, 1, endian, nails, buffer);
      //cout << "csize = " << mpz_sizeinbase(C.get_mpz_t(), 2) << endl;
      mpz_powm(m.get_mpz_t(), C.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
      //cout << "m = " << m.get_mpz_t() << endl;
      //cout << "c = " << C.get_mpz_t() << endl;
      mpz_export(buffer, &count, order, 1, endian, nails, m.get_mpz_t());
      //cout << "count = " << count << endl;
      outfile.write(buffer, count);
      memset(buffer, 0, BUFFER_SIZE);
    }
    cout << endl;
  }else cout << "Unable to open file!" << endl;
  cout << "Creating : " << PLAINFILE << endl;
  cout << "-----------------------------" << endl;

  delete [] buffer;
  infile.close();
  outfile.close();
}
