#pragma once
#include <string>
#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "pch.h"
using namespace boost::multiprecision;

class RSA
{
public:
	RSA(int coeff);
	~RSA();

	int512_t generatePrime();
	int512_t generateModulus();
	int512_t getEulerTotient() const;
	int512_t calculateEulerTotient();
	int512_t RSApowers(int512_t base, int512_t power) const;
	int512_t RSApowersImproved(int512_t base, int512_t power);
	int512_t doEuclidsExtendedAlgorithm();

	bool isPrime(int512_t number) const;
	bool combinationFound(int512_t power, int512_t exp);

	void generatePrivatePublicKeyPair();

	void encryptWAV(int count, int nb, char  buffer[512]);
	void decryptWAV(int count, int nb, char  buffer[512]);

	int512_t encryptText(int512_t plainText);
	int512_t decryptText(int512_t cipherText);

	int512_t getPrivateKey() const;
	int512_t getPublicKey() const;
	int512_t getPrimeQ() const;
	int512_t getPrimeP() const;

private:
	int512_t publicKey;
	int512_t privateKey;

	int512_t coefficient = 0;
	int512_t modulus;
	int512_t primeP;
	int512_t primeQ;
	int512_t eulerTotient;

	std::vector<int512_t> ans;
	std::vector<int> powerAnswerSeq;
};

