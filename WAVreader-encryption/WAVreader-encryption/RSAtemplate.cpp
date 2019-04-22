#include "pch.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <string>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include "RSA.h"

template <class intType>
struct Euclid
{
	Euclid(intType a, intType b)
	{
		base = a;
		divider = b;
	}

	intType base;
	intType divider;
};

/*HELPER FUNCTIONS*/

template <class intType> intType myPow(intType x, intType p)
{
	if (p == 0) return 1;
	if (p == 1) return x;

	intType tmp = myPow(x, p / 2);
	if (p % 2 == 0) return tmp * tmp;
	else return x * tmp * tmp;
}

template <class intType> bool RSA<intType>::combinationFound(intType power, intType exp)
{
	intType totalExp = 0;
	int count = this->ans.size() - 1;
	while (count >= 0)
	{
		intType tmp;
		if (count == 0)
		{
			tmp = 0;
		}
		else
		{
			tmp = myPow(2, count - 1);
		}

		totalExp += tmp;
		if (totalExp > power)
		{
			totalExp -= tmp;
		}
		else if (totalExp < power)
		{
			this->powerAnswerSeq.push_back(count);
		}
		else
		{
			this->powerAnswerSeq.push_back(count);
			return true;
		}
		count--;
	}
	powerAnswerSeq.clear();
	return false;
}

template <class intType> intType RSA<intType>::RSApowersImproved(intType base, intType power)
{
	intType exp = 0;
	while (!combinationFound(power, exp))
	{
		if (exp == 0)
		{
			this->ans.push_back(1);// 2^0
			exp = 1;
			continue;
		}
		intType answer = RSApowers(base, exp);
		exp *= 2;
		this->ans.push_back(answer);
	}

	intType endResult = 1;
	std::vector<int>::iterator it;
	for (it = this->powerAnswerSeq.begin(); it != this->powerAnswerSeq.end(); it++)
	{
		endResult = (endResult * ans.at(*it)) % this->modulus;
	}
	return endResult;
}


template <class intType>  intType RSA<intType>::RSApowers(intType x, intType p) const
{
	if (p == 0) return 1;
	if (p == 1) return x % this->modulus;

	intType tmp = RSApowers(x, p / 2) % this->modulus;
	if (p % 2 == 0) return tmp * tmp % this->modulus;
	else return x * tmp * tmp % this->modulus;
}

/*END OF HELPER FUNCTIONS*/



template <class intType> RSA<intType>::RSA()
{
	cout << "-------RSA CONFIG-------" << "\n\n";
	cout << "\nmodulus: " << generateModulus() << endl;
	cout << "Random Primes: " << getPrimeP() << " :: " << getPrimeQ() << endl;
	cout << "Euler Totient: " << calculateEulerTotient() << endl;
	generatePrivatePublicKeyPair();
	cout << "Public Key: " << getPublicKey() << endl;
	cout << "Private Key: " << getPrivateKey() << "\n\n";
}

template <class intType> RSA<intType>::RSA(intType publicKeyDefault) : publicKey(publicKeyDefault)
{
	cout << "-------RSA CONFIG-------" << "\n\n";
	cout << "modulus: " << generateModulusPublicKeyInitialized() << endl;
	cout << "Random Primes: " << getPrimeP() << " :: " << getPrimeQ() << endl;
	cout << "Euler Totient: " << calculateEulerTotient() << endl;
	doEuclidsExtendedAlgorithm();
	cout << "Public Key: " << getPublicKey() << endl;
	cout << "Private Key: " << getPrivateKey() << "\n\n";
}

template <class intType> intType RSA<intType>::generateModulusPublicKeyInitialized()
{
	while ( (this->primeP == this->primeQ) || (this->eulerTotient % this->publicKey == 0) )
	{
		this->primeP = generatePrime();
		this->primeQ = generatePrime();
		calculateEulerTotient();
	}
	return this->modulus = primeP * primeQ;
}

template <class intType> intType RSA<intType>::generateModulus()
{
	while ( this->primeP == this->primeQ )
	{
		this->primeP = generatePrime();
		this->primeQ = generatePrime();
	}
	return this->modulus = primeP * primeQ;
}

template <class intType> void RSA<intType>::generatePrivatePublicKeyPair()
{
	do
	{
		this->publicKey = generatePrime();
	} while (this->eulerTotient % this->publicKey == 0);
	doEuclidsExtendedAlgorithm();
}

template <class intType> intType RSA<intType>::generatePrime()
{
	boost::random::mt19937 gen(std::time(0));
	boost::random::uniform_int_distribution<intType> dist( 1, std::numeric_limits<short int>::max());
	intType randomNumber = dist(gen);
	if ((randomNumber % 2) == 0)
	{
		randomNumber++;
	}

	while (true)
	{
		if (isPrime(randomNumber))
		{
			return randomNumber;
		}
		else
		{
			randomNumber += 2;
		}
	}

	return 0;
}

template <class intType> bool RSA<intType>::isPrime(intType number) const
{
	if (number == 1) return false;
	if (number == 2) return true;

	for (int i = 2; i*i <= number; ++i)
	{
		if (number % i == 0)
		{
			return false;
		}
	}
	return true;
}
template <class intType> intType RSA<intType>::doEuclidsExtendedAlgorithm()
{
	Euclid<intType> leftColumn(this->eulerTotient, this->publicKey);
	Euclid<intType> rightColumn(this->eulerTotient, 1);
	intType division;
	intType tmp;
	while (leftColumn.divider != 1)
	{
		division = leftColumn.base / leftColumn.divider;
		tmp = leftColumn.base - (division * leftColumn.divider);
		leftColumn.base = leftColumn.divider;
		if (tmp < 0)
		{
			leftColumn.divider = (tmp % eulerTotient + eulerTotient) % eulerTotient;
		}
		else
		{
			leftColumn.divider = tmp;
		}

		tmp = rightColumn.base - division * rightColumn.divider;
		rightColumn.base = rightColumn.divider;
		if (tmp < 0)
		{
			rightColumn.divider = (tmp % eulerTotient + eulerTotient) % eulerTotient;
		}
		else
		{
			rightColumn.divider = tmp;
		}
	}
	this->privateKey = rightColumn.divider;
	return this->privateKey;
}

template <class intType> intType RSA<intType>::calculateEulerTotient()
{
	return eulerTotient = (primeP - 1) * (primeQ - 1);
}

template <class intType> std::vector<cpp_int> RSA<intType>::encryptWAV(std::string& buffer)
{	
	std::vector<cpp_int> mess( buffer.size() );

	for (int i = 0; i < buffer.size(); i++)
	{
		boost::multiprecision::cpp_int pow(this->publicKey);
		boost::multiprecision::cpp_int mod(this->modulus);
		boost::multiprecision::cpp_int base( (short)buffer[i] );
		boost::multiprecision::cpp_int result = powm(base, pow, mod);
		
		mess[i] = result;	
	}

	return mess;
}

template <class intType> std::vector<char> RSA<intType>::decryptWAV(std::vector<cpp_int>& vect)
{
	std::vector<char> mess( vect.size() );
	
	for (int i = 0; i < vect.size(); i++)
	{
		boost::multiprecision::cpp_int pow(this->privateKey);
		boost::multiprecision::cpp_int mod(this->modulus);
		boost::multiprecision::cpp_int base( vect[i] );
		boost::multiprecision::cpp_int result = powm(base, pow, mod);
		
		mess[i] = static_cast<char> (result);
	}

	return mess;
}

template <class intType> intType RSA<intType>::encryptText(intType plainText)
{
	intType message = RSApowers(plainText, publicKey);
	plainText = message;
	return plainText;
}

template <class intType> intType RSA<intType>::decryptText(intType cipherText)
{
	intType message = RSApowers(cipherText, privateKey);
	cipherText = message;
	return cipherText;
}


// GETTERS 
template <class intType> intType RSA<intType>::getPrimeQ() const
{
	return this->primeQ;
}

template <class intType> intType RSA<intType>::getPrimeP() const
{
	return this->primeP;
}

template<class intType>
intType RSA<intType>::getModulus() const
{
	return this->modulus;
}

template <class intType> intType RSA<intType>::getPrivateKey() const
{
	return this->privateKey;
}

template <class intType> intType RSA<intType>::getPublicKey() const
{
	return this->publicKey;
}

template <class intType> intType RSA<intType>::getEulerTotient() const
{
	return this->eulerTotient;
}
