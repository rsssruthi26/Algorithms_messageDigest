// You need to complete this program for a part of your first project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h>

//librarys added to meet project requirements
#include <time.h>
#include <fstream>

// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

//function prototypes
bool isPrime(BigUnsigned&);
BigUnsigned createBig();
BigUnsigned findCoPrime(BigUnsigned, BigUnsigned, int);

int main(){

	/* The library throws 'const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a 'try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try
	{
		std::ofstream p_q, e_n, d_n;

		//set the seed to get random numbers
		srand (time(NULL));

		//print out loading message.
		std::cout << "Generating Prime Number P. Please wait...\n";

		//generate prime numbers
		BigUnsigned bigPP = createBig();
	  while (!isPrime(bigPP))
	  {
	      bigPP = createBig();
	  }

		//print out loading message.
		std::cout << "Generating Prime Number Q. Please wait...\n";

		//generate prime number
		BigUnsigned bigPQ = createBig();
	  while (!isPrime(bigPQ))
	  {
	      bigPQ = createBig();
	  }

		std::cout << "Writing prime numbers to file.\n";

		//write the prime number p and q to file, then close the file
		p_q.open("p_q.txt");
		p_q << bigPP << '\n' << bigPQ;
		p_q.close();

		//calculate n and phi(n)
		BigUnsigned bigN = BigUnsigned(bigPP*bigPQ);
		BigUnsigned phiN = (bigPP-1)*(bigPQ-1);

		//create a random number e 3 digits long
		BigUnsigned bigE = BigUnsigned(rand() % 999 + 1000000000000);

		//find a valid big E for coprimes
		int failsafe = 0;
		bigE = findCoPrime(bigE,phiN,failsafe);

		//find d using modinv method created in the library
		BigUnsigned bigD = modinv(bigE, phiN);

		//write e and n to file
		e_n.open("e_n.txt");
		e_n << bigE << '\n' << bigN;
		e_n.close();

		//write d and n to file
		d_n.open("d_n.txt");
		d_n << bigD << '\n' << bigN;
		d_n.close();

		std::cout <<"Done generating private and public keys.\n"<<
		"Please copy \"e_n.txt\" and \"d_n.txt\" into the messageDigest435 directory before continuing.\n";

	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"<< err << std::endl;
	}
	return 0;
}
BigUnsigned createBig()
{
	// get random BigUnsigned int >= 512 bits
	 BigUnsigned bigInt512 = BigUnsigned(1);
	 while (bigInt512.bitLength() < 512)
	 {
			 bigInt512 = bigInt512*10 +rand();
	 }
	 return bigInt512;
}

bool isPrime(BigUnsigned& bigInt){
    // the a value in Fermat's test - (a ^ ( p - 1)) % p
    BigInteger aValue1 = 2, aValue2 = 3;

    // perform Fermat's test
    BigUnsigned remainder1 = modexp(aValue1, (bigInt - 1), bigInt);
    BigUnsigned remainder2 = modexp(aValue2, (bigInt - 1), bigInt);

    // if both remainders are 1, then Fermat's test is true / passed - number is prime
    if ((remainder1 == 1) && (remainder2 == 1))
        return true;
    else
        return false;
}

BigUnsigned findCoPrime(BigUnsigned e, BigUnsigned phiN, int failsafe)
{
	while(true)
	{
		//if e is not a co prime, add one and check again
		if(gcd(e,phiN)!=1)
			e +=1;
		//if e is a co prime, return e
		if(gcd(e,phiN) == 1)
			return e;

		//failsafe so that the loop doesnt get hung up on trying to find co primes.
		failsafe++;
		//break after 1000 failed tries to find a coprime and return e;
		if(failsafe >=1000)
		{
			std::cout << "WARNING No Coprimes"<<std::endl;
			return e;
		}
	}
}
