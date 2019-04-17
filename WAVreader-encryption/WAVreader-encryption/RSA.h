#pragma once
#include <string>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "pch.h"

using namespace boost::multiprecision;

template <class intType> class RSA
{
public:
	RSA(int coeff);
	~RSA();

	intType generatePrime();
	intType generateModulus();
	intType getEulerTotient() const;
	intType calculateEulerTotient();
	intType RSApowers(intType base, intType power) const;
	intType RSApowersImproved(intType base, intType power);
	intType doEuclidsExtendedAlgorithm();

	bool isPrime(intType number) const;
	bool combinationFound(intType power, intType exp);

	void generatePrivatePublicKeyPair();

	intType encryptWAV(int nb, short int buffer[512]);
	intType decryptWAV(int nb, short int buffer[512]);

	intType encryptText(intType plainText);
	intType decryptText(intType cipherText);

	intType getPrivateKey() const;
	intType getPublicKey() const;
	intType getPrimeQ() const;
	intType getPrimeP() const;

private:
	intType publicKey;
	intType privateKey;

	intType coefficient = 0;
	intType modulus;
	intType primeP = 0;
	intType primeQ = 0;
	intType eulerTotient;
	const long RANDOM_NUMBER_RANGE = 1000;
	std::vector<intType> ans;
	std::vector<int> powerAnswerSeq;
};

#include "RSAtemplate.cpp"