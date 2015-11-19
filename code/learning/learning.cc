#include <iostream>
#include <gmpxx.h>

using namespace std;

int main(){

	//Addition 
	mpz_class roo, foo, too; cout << roo << " " << foo << " " << too << endl; 
	foo = 111111111111;
	too = 222222222222;
	roo = foo + too;

	cout << roo << " = " << foo << " " << too << endl;
	
	//Square Root
	mpz_class squareRoot;
	mpz_class x = 100000000000000;
	squareRoot = sqrt(x);
	cout << "Square root of " << x << " is " << squareRoot << endl;

	//Absolute Value
	mpz_class absoluteValue;
	x -= 150000000000000;
	absoluteValue = abs(x); 
	cout << "Absolute value of " << x << " is " << absoluteValue << endl; 	

	//Greatest Common Divisor (GCD) 
	mpz_class a, b, c; 
	a = 100000000000000; 
	b = 500000000000;
	mpz_gcd (c.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
	cout << "GCD of " << a << " and  " << b << " is " << c << endl;

	//Negative
	mpz_neg(c.get_mpz_t(), a.get_mpz_t());
	cout << "Negative of " << a << " is " << c << endl;

	//Modular Arithmetic
	mpz_mod(c.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
	cout << a << " mod " << b << " is " << c << endl;

	//Exponents with modular arithmetic
	mpz_class r, base, exp, modular;
	base = 100; 
	exp = 3; 
	modular = 500000000; 
	mpz_powm(r.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), modular.get_mpz_t()); 
	cout << base << "^" << exp << " mod " << modular << " = " << r <<  endl;

	//Modular inverse
	mpz_class inverse;
	a = 19;
	b = 792;
	mpz_invert(inverse.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
	cout << "Inverse of " << a << " mod " << b << " is " << inverse <<  endl;
	c = inverse*a;
	mpz_mod(r.get_mpz_t(), c.get_mpz_t(), b.get_mpz_t());
	cout << "because " << a << " * " << inverse << " mod " << b << " = " << r << endl;

	//Next Prime
	mpz_class nextPrime;
	x = 452930459;
	mpz_nextprime(nextPrime.get_mpz_t(), x.get_mpz_t());
	cout << "Next prime after " << x << " is " << nextPrime << endl;

	//Bitwise exclusive-or (XOR)
	mpz_class op1, op2;
	op1 = 9;
	op2 = 15;
	mpz_xor(r.get_mpz_t(), op1.get_mpz_t(), op2.get_mpz_t());
	cout << op1 << " xor " << op2 << " = " << r << endl; 
   
	//Random numbers
	//gmp_randstate_t z;
	//gmp_randclass r1 (gmp_randinit_default);
	//cout << "Random number: " << r1 << endl; 

	return 0;
}

