#include "pch.h"
#include "RSA.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <string>
#include "pch.h"
#include <stdio.h>
#include "pch.h"
#include <cmath>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>


struct Euclid
{
	Euclid(int512_t a, int512_t b)
	{
		base = a;
		divider = b;
	}

	int512_t base;
	int512_t divider;
};

/*HELPER FUNCTIONS*/

int512_t myPow(int x, int p)
{
	if (p == 0) return 1;
	if (p == 1) return x;

	int512_t tmp = myPow(x, p / 2);
	if (p % 2 == 0) return tmp * tmp;
	else return x * tmp * tmp;
}

int512_t combine(int512_t a, int512_t b)
{
	int times = 1;
	while (times <= b)
		times *= 10;
	return a * times + b;
}

bool RSA::combinationFound(int512_t power, int512_t exp)
{
	int512_t totalExp = 0;
	int count = this->ans.size() - 1;
	while (count >= 0)
	{
		int512_t tmp;
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

int512_t RSA::RSApowersImproved(int512_t base, int512_t power)
{
	int512_t exp = 0;
	while (!combinationFound(power, exp))
	{
		if (exp == 0)
		{
			this->ans.push_back(1);// 2^0
			exp = 1;
			continue;
		}
		int512_t answer = RSApowers(base, exp);
		exp *= 2;
		this->ans.push_back(answer);
	}

	int512_t endResult = 1;
	std::vector<int>::iterator it;
	for (it = this->powerAnswerSeq.begin(); it != this->powerAnswerSeq.end(); it++)
	{
		endResult = (endResult * ans.at(*it)) % this->modulus;
	}
	return endResult;
}


int512_t RSA::RSApowers(int512_t x, int512_t p) const
{
	if (p == 0) return 1;
	if (p == 1) return x % this->modulus;

	int512_t tmp = RSApowers(x, p / 2) % this->modulus;
	if (p % 2 == 0) return tmp * tmp % this->modulus;
	else return x * tmp * tmp % this->modulus;
}

/*END OF HELPER FUNCTIONS*/



RSA::RSA(int k) : coefficient(k)
{
	//this->modulus = generateModulus();
	//generatePrivatePublicKeyPair();
}


RSA::~RSA()
{

}

int512_t RSA::doEuclidsExtendedAlgorithm()
{
	Euclid leftColumn(this->eulerTotient, this->publicKey);
	Euclid rightColumn(this->eulerTotient, 1);
	int512_t division;
	int512_t tmp;
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

int512_t RSA::generatePrime()
{
	int512_t randomNumer = rand() % (1 << 6);

	if ((randomNumer % 2) == 0)
	{
		randomNumer++;
	}

	while (true)
	{
		if (isPrime(randomNumer))
		{
			return randomNumer;
		}
		else
		{
			randomNumer += 2;
		}
	}

	return 0;
}

int512_t RSA::generateModulus()
{
	this->primeP = generatePrime();
	this->primeQ = generatePrime();

	return this->modulus = primeP * primeQ;
}

void RSA::generatePrivatePublicKeyPair()
{
	do
	{
		this->publicKey = generatePrime();
	} while (this->publicKey % this->eulerTotient == 0);
	doEuclidsExtendedAlgorithm();
}

int512_t RSA::calculateEulerTotient()
{
	return eulerTotient = (primeP - 1) * (primeQ - 1);
}

int512_t RSA::getEulerTotient() const
{
	return eulerTotient;
}

bool RSA::isPrime(int512_t number) const
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
std::string printCharArray(char text[], int size)
{
	std::string convertedText;
	for (int i = 0; i < size; i++)
	{
		convertedText += text[i];
	}
	return convertedText;
}

void RSA::encryptWAV(int nrBytesRead, char  buffer[512])
{
	std::string combinedBuffer = printCharArray(buffer, 512);
	
	for (int i = 0; i < nrBytesRead; i++)
	{
		//int512_t message = RSApowers(static_cast<int8_t>(buffer[i]), publicKey);
		int512_t message = RSApowersImproved(buffer[i], publicKey);
		buffer[i] = (char)message;
	}
}

void RSA::decryptWAV(int nrBytesRead, char buffer[512])
{
	for (int i = 0; i < nrBytesRead; i++)
	{
		int512_t message = RSApowers(static_cast<int8_t>(buffer[i]), privateKey);
		buffer[i] = (char)message;
	}
}

int512_t RSA::encryptText(int512_t plainText)
{
	int512_t message = RSApowers(plainText, publicKey);
	plainText = message;
	return plainText;
}

int512_t RSA::decryptText(int512_t cipherText)
{
	int512_t message = RSApowers(cipherText, privateKey);
	cipherText = message;
	return cipherText;
}


int512_t RSA::getPrimeQ() const
{
	return this->primeQ;
}

int512_t RSA::getPrimeP() const
{
	return this->primeP;
}

int512_t RSA::getPrivateKey() const
{
	return this->privateKey;
}

int512_t RSA::getPublicKey() const
{
	return this->publicKey;
}
