// WAVreader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "RSA.h"
#include <intrin.h>
#include<boost/ref.hpp>

using namespace std;


typedef struct header_file
{
	unsigned char chunk_id[4];							//Big-endian  "RIFF"
	unsigned char chunk_size[4];						//Little-endian "Size of File"
	unsigned char format[4];							//Big-endian  "WAVE"

	unsigned char subchunk1_id[4];						//Big-endian "fmt"
	unsigned char subchunk1_size[4];					//Little-endian  "size of subchunk1"
	unsigned char audio_format[2];						//Little-endian  "is 1 when PCM"
	unsigned char num_channels[2];						//Little-endian  "mono or stereo sound"
	unsigned char sample_rate[4];						//Little-endian
	unsigned char byte_rate[4];							//Little-endian
	unsigned char block_align[2];						//Little-endian
	unsigned char bits_per_sample[4];					//Little-endian

	unsigned char subchunk2_id[4];						//Big-endian   "data"
	unsigned char subchunk2_size[4];					//Little-endian
} header;

typedef struct header_file* header_p;

string printCharArray(unsigned char text[], int size)
{
	string convertedText;
	for (int i = 0; i < size; i++)
	{
		convertedText += text[i];
	}
	return convertedText;
}

unsigned int changeEndianness(unsigned char text[], int size)
{
	unsigned int convertedText;
	if (size == 4)
	{
		convertedText =	text[0] |
						(text[1] << 8) |
						(text[2] << 16) |
						(text[3] << 24);
    }
	else if (size == 2)
	{
		convertedText = text[0] |
						(text[1] << 8);
	}
	//return _byteswap_ushort(text);
	return convertedText;
}


void readHeader(const header_p &meta, FILE * infile, FILE * outfile)
{
	fread(meta, 1, sizeof(header), infile);
	fwrite(meta, 1, sizeof(*meta), outfile);
	/*cout << " ------buffer--- " << endl;
	for (int i = 0; i < sizeof(header); i++)
	{
		cout << "position(i): " << i << " : " << (int)buffer[i] << endl;
	}*/


	cout << " Chunk ID: " << printCharArray(meta->chunk_id, 4) << endl;
	cout << " Chunk Size: " << changeEndianness(meta->chunk_size, 4) << " bytes " << endl;
	cout << " Chunk Format: " << printCharArray(meta->format, 4) << endl;

	cout << " SubChunk_1 ID: " << printCharArray(meta->subchunk1_id, 4) << endl;
	cout << " SubChunk_1 Size: " << changeEndianness(meta->subchunk1_size, 4) << " bytes " << endl;
	cout << " SubChunk_1 Format: " << changeEndianness(meta->audio_format, 2) << endl;
	cout << " Number of Channels: " << changeEndianness(meta->num_channels, 2) << endl;
	cout << " Sample Rate: " << changeEndianness(meta->sample_rate, 4) << " Hz" << endl;
	cout << " Byte Rate: " << changeEndianness(meta->byte_rate, 4) << endl;
	cout << " Block Allign: " << changeEndianness(meta->sample_rate, 2) << endl;
	cout << " Bits per Sample: " << changeEndianness(meta->bits_per_sample, 4) << " bits" << endl;


	cout << " SubChunk_2 ID: " << printCharArray(meta->subchunk2_id, 4) << endl;
	cout << " SubChunk_2 Size: " << changeEndianness(meta->subchunk2_size, 4) << " bytes" << endl;
}

void printRSA(RSA& rsa)
{
	cout << "-------RSA CONFIG-------" << endl;
	cout << "modulus: " << rsa.generateModulus() << endl;
	cout << "Random Primes: " << rsa.getPrimeP() << " :: " << rsa.getPrimeQ() << endl;
	cout << "Euler Totient: " << rsa.calculateEulerTotient() << endl;
	rsa.generatePrivatePublicKeyPair();
	cout << "Public Key: " << rsa.getPublicKey() << endl;
	cout << "Private Key: " << rsa.getPrivateKey() << endl;
}

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
	srand(time(NULL));
	constexpr int BUFSIZE = 512;								// BUFSIZE can be changed according to the frame size required (eg:512)
	int nrBytesRead;											// variable storing number of bytes returned
	int count = 0;												// For counting number of frames in wave file.

	constexpr char filepath[] = { "pcm16.wav" };
	char buffer[BUFSIZE];										// short int used for 16 bit as input data format is 16 bit PCM audio
	char noiseBuffer[BUFSIZE];
	RSA rsa(3);
	FILE * infile = fopen(filepath, "rb");						// Open wave file in read mode
	FILE * outfile = fopen("Output.wav", "wb");					// Create output ( wave format) file in write mode

	header_p meta = (header_p)malloc(sizeof(header));			// header_p points to a header struct that contains the wave file metadata fields
	printRSA(rsa);
	/*char* tmp;
	string plainText = "Hello World";
	tmp = convertToASCII(plainText);*/
	/*cout << "my PlainText in ASCII: " << (int)(*tmp) << endl;
	int512_t encryptedText = rsa.encryptText((int)(*tmp));
	cout << "my encryptedText: " << encryptedText << endl;
	cout << "my decryptedText: " << rsa.decryptText(encryptedText) << endl;*/
	
	//int512_t ans = rsa.RSApowersImproved(2, 4);
	//cout << "RSA_IMPROVED: " << ans << endl;

	if (infile)
	{
		readHeader(meta, infile, outfile);

		while (!feof(infile))
		{
			nrBytesRead = fread(buffer, sizeof(char), BUFSIZE, infile);			// Reading data from infile to buffer in chunks of BUFSIZE
			count++;															// Incrementing Number of frames

			rsa.encryptWAV(nrBytesRead, buffer);
			//rsa.decryptWAV(nrBytesRead, buffer);

			fwrite(noiseBuffer, sizeof(char), nrBytesRead, outfile);			// Writing read data into output file
		}
		cout << "FRAMES: " << count << endl;
	}
	else
	{
		cout << "can't open file";
	}

	return 0;
}

