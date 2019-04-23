#include "pch.h"
#include <iostream>
#include "RSA.h"
#include "XOR.h"
#include <vector>
#include <string>
#include "Parser.h"
#include "WAVreader.h"

using namespace std;


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

int main()
{		
	string filepath = "nature.wav";				    				// choose .wav file to be parsed
	
	constexpr int BUFFERSIZE = BUFSIZ;							    // data chunk size

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

	
	if (parser->inputFile)
	{								
		int nrBytesRead = 0;																				// variable storing number of bytes returned
		int count = 0;																						// frame counter

		string buffer;
		buffer.resize(BUFFERSIZE);
		
		string sineBuffer;
		sineBuffer.resize(BUFFERSIZE);

		std::vector<char> vect;
		std::vector<cpp_int> mess;
		
		while ( !feof(parser->inputFile) )
		{
			nrBytesRead = std::fread(&buffer[0], 1, buffer.size(), parser->inputFile);						// Reading data from infile to buffer in chunks of BUFSIZE
			count++;
			
			sineBuffer = addSineToWave(count, nrBytesRead, buffer, parser, 3, 7, 256);
			std::fwrite(&sineBuffer[0], 1, nrBytesRead, parser->sineOutput);
			
			xor.encryptXorWav(buffer);
			std::fwrite(&buffer[0], 1, nrBytesRead, parser->xorEncryptedOutput);
			
			xor.encryptXorWav(buffer);
			std::fwrite(&buffer[0], 1, nrBytesRead, parser->xorOutput);
			
			mess = rsa.encryptWAV(buffer);
			std::fwrite(mess.data(), sizeof mess[0], mess.size(), parser->rsaEncryptedOutput);
			
			vect = rsa.decryptWAV(mess);
			std::fwrite(vect.data(), 1, vect.size(), parser->rsaOutput);
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

