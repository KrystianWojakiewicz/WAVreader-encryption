#pragma once
#include "pch.h"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

#include "RSA.h"
#include "XOR.h"
#include "Parser.h"
#include "WAVreader.h"
#include "AESAlgorithm.h"



std::string addSineToWave(int count, int nrBytesRead, std::string sineBuffer, Parser* parser, int signalStartSec, int signalEndSec, int period)
{
	int ampl = 0;
	unsigned int byteRate = parser->changeEndianness(parser->meta->byte_rate, 4);

	int start = byteRate * signalStartSec / BUFSIZ;
	int end = byteRate * signalEndSec / BUFSIZ;

	if (count > start  && count < end)
	{
		for (int i = 0; i < nrBytesRead; i++)
		{
			if (i % period < period / 2)
			{
				sineBuffer[i] = ampl++;
			}
			else
			{
				sineBuffer[i] = ampl--;
			}
		}
	}
	return sineBuffer;
}

void sampleRSA(RSA<boost::multiprecision::uint1024_t> &rsa)
{
	std::cout << "-----------RSA-------------" << "\n\n";

	std::string plainText = "Hello World!";
	std::cout << "my PlainText in ASCII: " << plainText << std::endl;

	std::vector<cpp_int> encryptedVec = rsa.encryptWAV(plainText);
	std::string encryptedText;
	for (int i = 0; i < encryptedVec.size(); i++)
	{
		encryptedText += boost::lexical_cast<std::string>(encryptedVec[i]);
	}
	std::cout << "my encryptedText: " << encryptedText << std::endl;


	std::vector<char> decryptedVec = rsa.decryptWAV(encryptedVec);
	std::string decryptedText;
	for (int i = 0; i < decryptedVec.size(); i++)
	{
		decryptedText += boost::lexical_cast<std::string>(decryptedVec[i]);
	}
	std::cout << "my decryptedText: " << decryptedText << "\n\n";
}

void sampleXOR(XOR<boost::multiprecision::uint1024_t> &xor)
{
	std::cout << "-----------XOR-------------" << "\n\n";
	std::string plainTextXor = "0123456789";
	std::cout << "my PlainText for XOR: " << plainTextXor << std::endl;
	std::cout << "cipher: " << xor.getCipher() << std::endl;
	std::cout << "my encryptedXOR: " << xor.encryptXorWav(plainTextXor) << std::endl;
	std::cout << "my decryptedXOR: " << xor.encryptXorWav(plainTextXor) << "\n\n";
}

void sampleAES(AESAlgorithm &aes)
{
	std::cout << "-----------AES-------------" << "\n\n";
	std::string plainText = "HELLO WORLD!";
	std::cout << "Encrypted AES: " << aes.encryptWAV(plainText, plainText.size()); std::cout << "\n";
	std::cout << "Decrypted AES: " << aes.decryptWAV(plainText.size()); std::cout << "\n";
}



int main()
{		
	std::string filepath = "testSample.wav";				    				// choose .wav file to be parsed
	constexpr int BUFFERSIZE = BUFSIZ;											// data chunk size
	
	Parser * parser = new Parser(filepath);
	
	RSA <uint1024_t> rsa(3);
	XOR<uint1024_t> xor;
	AESAlgorithm aes;

	
	std::cout << "\n\nSample encryption/decryption\n";
	sampleRSA(rsa);
	sampleXOR(xor);
	sampleAES(aes);
	
	if (parser->inputFile)
	{								
		int nrBytesRead = 0;																				// variable storing number of bytes returned
		int count = 0;																						// frame counter

		std::string buffer;
		buffer.resize(BUFFERSIZE);
		
		std::string sineBuffer;
		sineBuffer.resize(BUFFERSIZE);

		std::vector<char> vect;
		std::vector<cpp_int> mess;
		
		auto start = std::chrono::system_clock::now();														// TIME START
		while ( !feof(parser->inputFile) )
		{
			nrBytesRead = std::fread(&buffer[0], 1, buffer.size(), parser->inputFile);						// Reading data from infile to buffer in chunks of BUFSIZE
			count++;
			
			// Adding Sine
			sineBuffer = addSineToWave(count, nrBytesRead, buffer, parser, 3, 7, 256);
			std::fwrite(&sineBuffer[0], 1, nrBytesRead, parser->sineOutput);
			
			// XOR encryption/decryption
			xor.encryptXorWav(buffer);
			std::fwrite(&buffer[0], 1, nrBytesRead, parser->xorEncryptedOutput);
			
			xor.encryptXorWav(buffer);
			std::fwrite(&buffer[0], 1, nrBytesRead, parser->xorOutput);
			
			// RSA encryption/decryption
			mess = rsa.encryptWAV(buffer);
			std::fwrite(mess.data(), sizeof mess[0], mess.size(), parser->rsaEncryptedOutput);
			
			vect = rsa.decryptWAV(mess);
			std::fwrite(vect.data(), 1, vect.size(), parser->rsaOutput);

			// AES encryption/decryption
			std::string aesCipherText = aes.encryptWAV(buffer, nrBytesRead); 
			std::fwrite(&aesCipherText[0], 1, nrBytesRead, parser->aesEncryptedOutput);

			std::string aesDecrypted = aes.decryptWAV(nrBytesRead);
			std::fwrite(&aesDecrypted[0], 1, nrBytesRead, parser->aesOutput);
		}

		std::cout << "FRAMES: " << count << std::endl;

		auto end = std::chrono::system_clock::now();														// TIME STOP
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	}
	else
	{
		std::cout << "can't open file";
	}
	
	delete parser;

	return 0;
}


