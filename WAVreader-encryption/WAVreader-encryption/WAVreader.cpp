// WAVreader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "RSA.h"
#include "XOR.h"
#include <fstream>
#include <intrin.h>
#include <vector>
#include <boost/ref.hpp>
#include <boost/iostreams/read.hpp>
#include <boost/iostreams/write.hpp>
#include <boost/iostreams/close.hpp>
#include <ostream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>

using namespace std;
namespace io = boost::iostreams;

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
}header;

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

template<class intType>
void printRSA(RSA<intType>& rsa)
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

uint128_t combine1(std::vector<unsigned short>& vector)
{
	//unsigned int A[] = { 156,23,212,4,12 };
	uint128_t result = 0;
	const int length_A = vector.size();//sizeof(A) / sizeof(*A);
	for (int i = 0; i < length_A; i++)
	{
		result += vector[i];
		if (i < length_A - 1)
		{
			int digits = (int)ceil(log10((double)vector[i + 1]));
			result *= (int)pow(10, digits);
		}
	}
	//cout << "A: " << result << endl;
	return result;
}

int128_t combine(int128_t result, short int b)
{
	short int times = 1;
	while (times <= b)
		times *= 10;
	return result = result * times + b;
}

uint128_t combine3(std::vector<unsigned short>& vector)
{
	uint128_t result;
	for (int i = 0; i < vector.size(); i++)
	{
		result *= 10;
		result += vector[i];
	}
	return result;
}

string printCharArray(short int text[], int size)
{
	string convertedText;
	for (int i = 0; i < size; i++)
	{
		convertedText += text[i];
	}
	return convertedText;
}

int main()
{
	constexpr char filepath[] = { "pcm16.wav" };
	constexpr int BUFFERSIZE = 8;
	char buffer[BUFFERSIZE];									// short int used for 16 bit as input data format is 16 bit PCM audio
	int128_t cipher("4454354534434343353445435454435435345345534");
	RSA <int128_t> rsa(3);
	XOR<uint128_t> xor;
	
	FILE * infile = fopen(filepath, "rb");						// Open wave file in read mode
	FILE * outfile = fopen("xortest.wav", "wb");					// Create output ( wave format) file in write mode

	header_p meta = (header_p)malloc(sizeof(header));			// header_p points to a header struct that contains the wave file metadata fields
	printRSA(rsa);
 	char plainText[] = { "Hello World" };
	cout << "\n my PlainText in ASCII: " << (unsigned int)(*plainText) << endl;
	cout << "-----------TEXT-------------" << endl;
	int128_t encryptedText = rsa.encryptText((unsigned int)(*plainText));
	cout << "my encryptedText: " << encryptedText << endl;
	cout << "my decryptedText: " << rsa.decryptText(encryptedText) << "\n\n";

	/*cout << "-----------WAV-------------" << endl;
	int512_t encryptedWAV = rsa.encryptWAV(11, (short int*)plainText);
	cout << "my encryptedWAV: " << encryptedWAV << endl;
	cout << "my decryptedWAV: " << rsa.decryptWAV(11, (short int*)plainText) << "\n\n";*/
	
	/*int128_t plainTextXOR[] = {1, 2, 3, 4, 5, 6};
	cout << "cipher: " << xor.getCipher() << endl;
	cout << "-----------XOR-------------" << endl;
	cout << "my encryptedXOR: " << *(xor.encryptXOR(plainTextXOR, 6)) << endl;
 	cout << "my decryptedXOR: " << *(xor.encryptXOR(plainTextXOR, 6)) << "\n\n";

	cout << "============HEADER============" << "\n\n";*/
	if (infile)
	{
		readHeader(meta, infile, outfile);
		int nrBytesRead = 0;																			// variable storing number of bytes returned
		int count = 0;																					// For counting number of frames in wave file.
		std::vector<char> buff(BUFFERSIZE);
		std::vector<int> buff2(BUFFERSIZE);
		while (!feof(infile))
		{
			string in;
			in.resize(BUFFERSIZE);
			nrBytesRead = std::fread(buff.data(), 1, buff.size(), infile);							// Reading data from infile to buffer in chunks of BUFSIZE
			//nrBytesRead = std::fread(&in[0], 1, in.size(), infile);
			count++;
			string boost;
			//cout << "sizeof buff[0]: " << sizeof buff[0];
			for (auto element : buff)
			{
				//cout << "before combine: " << element << endl;
				//cout << "string in: " << in << endl;
			}
			//uint128_t a = combine3(buff);
			//cout << "after combine: " << a << endl;
			for (int i = 0; i < nrBytesRead; i++)
			{
				boost += boost::lexical_cast<string>(buff[i]);
				//cout << "after combine: " << boost << endl;
			}
			
			for (int i = 0; i < nrBytesRead; i++)
			{
				//cout << buffer[i] << endl;
			}
			//xor.encryptXorWav(a);
			//xor.encryptXorWav(a);
			//xor.encryptXorWav(a);
			//xor.encryptXOR(buffer, nrBytesRead);
			//xor.encryptXOR(buffer, nrBytesRead);
			//uint128_t a = boost::lexical_cast<uint128_t>(boost);
			//if(count <5000)
			//std::reverse(boost.begin(), boost.end());
			fwrite(&boost[0], 1, nrBytesRead, outfile);									// Writing read data into output file
			//fwrite(&in[0], 1, nrBytesRead, outfile);
		}
		cout << "FRAMES: " << count << endl;
	}
	else
	{
		cout << "can't open file";
	}
	fclose(infile);
	fclose(outfile);

	return 0;
}

