#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "RSA.h"
#include "XOR.h"
#include <fstream>
#include <vector>
#include "Parser.h"

using namespace std;


int main()
{
	string filepath = "bass.wav";				    			// choose .wav file to be parsed
	
	constexpr int BUFFERSIZE = 512;							    // data chunk size

	RSA <cpp_int> rsa(3);
	XOR<uint512_t> xor;
	Parser * parser = new Parser(filepath);
	
	
	cout << "-----------RSA-------------" << "\n\n";
	char plainText[] = { "H" };
	cout << "my PlainText in ASCII: " << (unsigned int)(*plainText) << endl;
	cpp_int encryptedText = rsa.encryptText((unsigned int)(*plainText));
	cout << "my encryptedText: " << encryptedText << endl;
	cout << "my decryptedText: " << rsa.decryptText(encryptedText) << "\n\n";
	
	cout << "-----------XOR-------------" << "\n\n";
	string plainTextXor = "0123456789";
	cout << "cipher: " << xor.getCipher() << endl;
	cout << "my encryptedXOR: "  << xor.encryptXorWav(plainTextXor) << endl;
 	cout << "my decryptedXOR: " << xor.encryptXorWav(plainTextXor) << "\n\n";

	
	if (parser->xorInput)
	{								
		int nrBytesRead = 0;																				// variable storing number of bytes returned
	
		string buffer;
		buffer.resize(BUFFERSIZE);
		
		std::vector<char> vect;
		std::vector<cpp_int> mess;
		
		while ( !feof(parser->xorInput) )
		{
			nrBytesRead = std::fread(&buffer[0], 1, buffer.size(), parser->xorInput);					// Reading data from infile to buffer in chunks of BUFSIZE
			
			
			xor.encryptXorWav(buffer);
			std::fwrite(&buffer[0], 1, nrBytesRead, parser->xorEncryptedOutput);
			
			xor.encryptXorWav(buffer);
			std::fwrite(&buffer[0], 1, nrBytesRead, parser->xorOutput);
			
			mess = rsa.encryptWAV(buffer);
			std::fwrite(mess.data(), sizeof mess[0], mess.size(), parser->rsaEncryptedOutput);
			
			vect = rsa.decryptWAV(mess);
			std::fwrite(vect.data(), 1, vect.size(), parser->rsaOutput);
		}
	}
	else
	{
		cout << "can't open file";
	}
	
	delete parser;

	return 0;
}

