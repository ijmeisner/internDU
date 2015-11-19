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
char* process(char* str);

using namespace std;

#define BIT_STRENGTH 2048			//Size of modulus (n) in bits
#define READ_IN 128				//in bytes
#define MESSAGE "billOfRights.txt"
#define CIPHERTEXT "cipherText.txt"
#define PLAINTEXT "plaintext.txt"

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
  char* msg;
  mpz_class c, M;
  int tmp;
  char tmps[4];

  cout << "Encrypting : " << MESSAGE << endl;
  cout << "-----------------------------" << endl;

  ifstream infile (MESSAGE, ios::in|ios::binary|ios::ate);
  ofstream outfile(CIPHERTEXT, ios::out|ios::binary);
  ofstream out(MESSAGE, ios::out|ios::binary|ios::app);
  if(infile.is_open() && outfile.is_open()){
    readSize = READ_IN;
    fileSize = infile.tellg();
    while(fileSize % readSize != 0){
      out << ' ';
      fileSize = out.tellp();
    }
    out.close();

    cout << "Read-in Size: " << readSize << " bytes / " << readSize*8 << " bits" << endl;
    cout << "File Size: " << fileSize << " bytes / " << fileSize*8 << " bits" << endl << endl;
    
    msg = new char [readSize];
    infile.seekg(0, ios::beg);

    while(infile.read(msg, readSize)){
      char* intmsg = new char[readSize*3 + 1];
      strcpy(intmsg,"");
      for(int j=0; j<readSize; j++){
        tmp = (int)msg[j];
        sprintf(tmps,"%03d",tmp); // print it in a 3 character wide format
        strcat(intmsg,tmps);
      }
      mpz_set_str(M.get_mpz_t(),intmsg,10);
      mpz_powm(c.get_mpz_t(), M.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
      outfile << c << endl << endl;
      delete [] intmsg;
    }
  }else cout << "Unable to open file!" << endl;

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
  char* cipherText;
  char* plainText;
  char* intcipher;
  char* decrypted = new char[1000];
  mpz_class m, C;
  char tmp;

  cout << "Decrypting : " << CIPHERTEXT << endl;
  cout << "-----------------------------" << endl;

  ifstream infile(CIPHERTEXT, ios::in|ios::binary|ios::ate);
  ofstream outfile(PLAINTEXT, ios::out|ios::binary);
  if(infile.is_open() && outfile.is_open()){
    readSize = READ_IN;
    fileSize = infile.tellg();
    cout << "Read-in Size: " << readSize << " bytes / " << readSize*8 << " bits" << endl;
    cout << "File Size: " << fileSize << " bytes / " << fileSize*8 << " bits" << endl << endl;

    //cipherText = new char[readSize];

    infile.seekg (0, ios::beg);
    //while(infile.read(cipherText, readSize)){
    while(infile >> C){
      mpz_powm(m.get_mpz_t(), C.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());
      mpz_get_str(decrypted, 10, m.get_mpz_t());
      plainText = process(decrypted);
      for(int k=0; k<readSize; k++){
        outfile << plainText[k];
      }
    }
  }else cout << "Unable to open file!" << endl;
  cout << "Creating : " << PLAINTEXT << endl;
  delete [] decrypted;
  infile.close();
  outfile.close();
}

//
// This function processes the letters related to the ASCII integers
// that are decrypted and returns a pointer to the char array holding the letters
//
char* process(char* str)
{
  unsigned int i=0;
  unsigned int j=0;
  int tmpnum;
  char strmod[1000];
  char* letters = new char[1000];

 /* make the message length an integral multiple
    of 3 by adding zeroes to the left if required */

  if(strlen(str)%3 == 1){
      strcpy(strmod,"00");
      strcat(strmod,str);
  }
  else if(strlen(str)%3 == 2){
      strcpy(strmod,"0");
      strcat(strmod,str);
  }
  else
      strcpy(strmod,str);

  while(i<=strlen(strmod)-3){
      tmpnum = strmod[i] - 48;
      tmpnum = 10*tmpnum + (strmod[i+1] - 48);
      tmpnum = 10*tmpnum + (strmod[i+2] - 48);

      letters[j] = tmpnum;
      i += 3;
      j++;
  }
  return letters;
}
