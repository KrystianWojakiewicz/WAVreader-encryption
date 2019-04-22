#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "RSA.h"
#include "XOR.h"
#include <fstream>
#include <intrin.h>
#include <vector>
#include "Parser.h"


using namespace std;



char* convertToASCII(string& plainText)
{
	char* newText = new char[plainText.length() + 1];
	for (int i = 0; i < plainText.length(); i++)
	{
		newText[i] = (int)plainText.at(i);
	}
	newText[plainText.length()] = '\0';
	return newText;
}


int main()
{
	string filepath = "Bowed-Bass-C2.wav";
	

	constexpr int BUFFERSIZE = 512;

	RSA <uint128_t> rsa(3);
	XOR<uint512_t> xor;
	Parser * parser = new Parser(filepath);
	
 	char plainText[] = { "Hello World" };
	cout << "\n my PlainText in ASCII: " << (unsigned int)(*plainText) << endl;
	cout << "-----------TEXT-------------" << endl;
	uint128_t encryptedText = rsa.encryptText((unsigned int)(*plainText));
	cout << "my encryptedText: " << encryptedText << endl;
	cout << "my decryptedText: " << rsa.decryptText(encryptedText) << "\n\n";
	
	/*int128_t plainTextXOR[] = {1, 2, 3, 4, 5, 6};
	cout << "cipher: " << xor.getCipher() << endl;
	cout << "-----------XOR-------------" << endl;
	cout << "my encryptedXOR: " << *(xor.encryptXOR(plainTextXOR, 6)) << endl;
 	cout << "my decryptedXOR: " << *(xor.encryptXOR(plainTextXOR, 6)) << "\n\n";

	cout << "============HEADER============" << "\n\n";*/
	
	if (parser->xorInput && parser->rsaInput)
	{
		int nrBytesReadRSA = 0;																			// variable storing number of bytes returned
		int nrBytesReadXor = 0;
		int count = 0;																					// For counting number of frames in wave file.
		std::vector<short int> rsaBuffer(BUFFERSIZE);
		string xorBuffer;
		xorBuffer.resize(BUFFERSIZE);
		std::vector<char> vect;
		while ( !feof(parser->rsaInput) || !feof(parser->xorInput) )
		{
			
			nrBytesReadXor = std::fread(&xorBuffer[0], 1, xorBuffer.size(), parser->xorInput);									// Reading data from infile to buffer in chunks of BUFSIZE
			//infile -= nrBytesReadXor;
			nrBytesReadRSA = std::fread(rsaBuffer.data(), sizeof(short int), rsaBuffer.size(), parser->rsaInput);
			count++;
			
			xor.encryptXorWav(xorBuffer);
			std::fwrite(&xorBuffer[0], 1, nrBytesReadXor, parser->xorEncryptedOutput);
			
			xor.encryptXorWav(xorBuffer);
			std::fwrite(&xorBuffer[0], 1, nrBytesReadXor, parser->xorOutput);
			
			//rsa.encryptWAV(nrBytesReadRSA, rsaBuffer);
			string message = rsa.encryptWAV2(xorBuffer, vect);
			//rsa.decryptWAV(nrBytesReadRSA, rsaBuffer);
			//message = rsa.decryptWAV2(message, vect);
			std::fwrite(vect.data(), 1, vect.size(), parser->rsaEncryptedOutput);

			//std::fwrite(&message[0], 1, message.size(), parser->rsaEncryptedOutput);
		   // message = rsa.decryptWAV2(message);
			//std::fwrite(&message[0], 1, message.size(), parser->rsaOutput);
			//std::fwrite(rsaBuffer.data(), sizeof(short int), nrBytesReadRSA, parser->rsaEncryptedOutput);
		}
		cout << "FRAMES: " << count << endl;
	}
	else
	{
		cout << "can't open file";
	}
	
	delete parser;

	return 0;
}

