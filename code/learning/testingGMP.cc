#include <iostream>
#include <gmpxx.h>

using namespace std;

int main(void){

	mpz_class x, y, z;

	x = 999999999999;
	y = 111111111111;
	z = x+y;
	cout << x << endl;
	cout << " + " << endl;
	cout << y << endl;
	cout << "-----------------" << endl;
	cout << z << endl;

	return 0;
}
