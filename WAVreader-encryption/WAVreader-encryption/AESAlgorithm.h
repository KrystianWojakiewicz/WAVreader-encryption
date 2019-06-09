#pragma once

#include "modes.h"
#include "aes.h"
#include "filters.h"
#include "osrng.h"


using CryptoPP::AutoSeededRandomPool;
using CryptoPP::SecByteBlock;


class AESAlgorithm
{
public:
	AESAlgorithm()
	{
		SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
		rnd.GenerateBlock(key, key.size());
		this->key = key;

		SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
		rnd.GenerateBlock(iv, iv.size());
		this->iv = iv;
	}

	void EncryptEAS()
	{
		CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(key, key.size(), iv);
		cfbEncryption.ProcessData(this->myCipherText, this->myPlainText, 12);
		
		printText(this->myCipherText);
	}

	void DecryptEAS()
	{
		CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption(key, key.size(), iv);
		cfbDecryption.ProcessData(this->myDecryptedText, this->myCipherText, 12);

		printText(this->myDecryptedText);
	}

private:
	std::string ciphertext, plaintext, decryptedText;
	AutoSeededRandomPool rnd;
	SecByteBlock key, iv;
	CryptoPP::byte myPlainText[12] = "Hello World";
	CryptoPP::byte myCipherText[12], myDecryptedText[12];
	


	void printText(CryptoPP::byte* text)
	{
		for (int i = 0; i < std::strlen((char*)text) + 1; i++)
		{
			std::cout << static_cast<CryptoPP::byte>(text[i]);
		}
		std::cout << std::endl;
	}
};