#pragma once

#include "modes.h"
#include "aes.h"
#include "filters.h"
#include "osrng.h"


using CryptoPP::AutoSeededRandomPool;
using CryptoPP::SecByteBlock;
using CryptoPP::byte;

class AESAlgorithm
{
public:
	AESAlgorithm()
	{
		AutoSeededRandomPool rnd;

		SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
		rnd.GenerateBlock(key, key.size());
		this->key = key;

		SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);
		rnd.GenerateBlock(iv, iv.size());
		this->iv = iv;
	}

	std::string encryptWAV(std::string& buffer, int bufferSize)
	{
		SecByteBlock plainText((const byte*)buffer.data(), bufferSize);
		
		SecByteBlock output(bufferSize);
		CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(key, key.size(), iv);
		cfbEncryption.ProcessData(output, (const byte*)buffer.data(), bufferSize);
		this->myCipherText = output;
		
		return std::string( (const char*)output.data(), output.size() );
	}

	std::string decryptWAV(int bufferSize)
	{
		SecByteBlock output(bufferSize);
		CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbEncryption(key, key.size(), iv);
		cfbEncryption.ProcessData(output, this->myCipherText, bufferSize);
		this->myDecryptedText = output;

		return std::string( (const char*)output.data(), output.size() );
	}


private:
	SecByteBlock key, iv, myCipherText, myDecryptedText;
	
};