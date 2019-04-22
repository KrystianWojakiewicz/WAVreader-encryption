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
	RSA(intType publicKeyDefault);
	RSA();

	intType generatePrime();
	intType generateModulus();
	intType generateModulusPublicKeyInitialized();

	
	intType calculateEulerTotient();
	intType RSApowers(intType base, intType power) const;
	intType RSApowersImproved(intType base, intType power);
	intType doEuclidsExtendedAlgorithm();

	bool isPrime(intType number) const;
	bool combinationFound(intType power, intType exp);

	void generatePrivatePublicKeyPair();

	std::vector< short int> encryptWAV(int nb, std::vector< short int>& buffer);
	std::vector< short int> decryptWAV(int nb, std::vector< short int>& buffer);

	std::string encryptWAV2(std::string& buffer, std::vector<char>& mess);
	std::string decryptWAV2(std::string& buffer, std::vector<char>& vect);

	intType encryptText(intType plainText);
	intType decryptText(intType cipherText);

	intType getPrivateKey() const;
	intType getPublicKey() const;
	intType getPrimeQ() const;
	intType getPrimeP() const;
	intType getModulus() const;
	intType getEulerTotient() const;

private:
	intType publicKey;
	intType privateKey;

	intType modulus;
	intType primeP = 0;
	intType primeQ = 0;
	intType eulerTotient;
	
	std::vector<intType> ans;
	std::vector<int> powerAnswerSeq;
};

#include "RSAtemplate.cpp"