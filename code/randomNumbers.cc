// Generate random number
// Isaac Meisner
// 6/23/15

#include <iostream>
#include <gmpxx.h>

mpz_class generateRandom();
//void initRandom();

using namespace std;

#define BIT_STRENGTH 2048

int main(void){

	//mpz_urandomb()
	mpz_class a, b ,c;
	srand((unsigned)time(0));
	//initRandom();
	a = generateRandom();
	cout << "Random Number 1: " << a << endl;
	b = generateRandom();
	cout << "Random Number 2: " << b << endl;
	c = generateRandom();
	cout << "Random Number3 : " << c << endl;

	return 0;
}

void initRandom()
{
  sleep(1); //Avoids calls in the same second
  unsigned int time_elapsed;
  time((time_t*)&time_elapsed);
  srand(time_elapsed);
}

mpz_class generateRandom()
{
	mpz_class ran;
	gmp_randclass rr(gmp_randinit_default);
	rr.seed(rand());
	ran = rr.get_z_bits(BIT_STRENGTH);
	//long int random=ran.get_ui();

	return ran;
}
