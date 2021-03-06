#pragma once

#include <iostream>
#include <stdio.h>
#include "RSA.h"
#include "XOR.h"
#include <fstream>
#include <vector>
#include <string>

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
	unsigned char bits_per_sample[2];					//Little-endian

	unsigned char subchunk2_id[4];						//Big-endian   "data"
	unsigned char subchunk2_size[4];					//Little-endian
}header;

typedef struct header_file* header_p;

class Parser
{
public:
	Parser(std::string filepath);
	~Parser();

	FILE * inputFile;
	
	FILE * xorOutput;
	FILE * xorEncryptedOutput;
	
	FILE * rsaOutput;
	FILE * rsaEncryptedOutput;

	FILE * aesOutput;
	FILE * aesEncryptedOutput;

	FILE * sineOutput;

	header_p meta;

	unsigned int changeEndianness(unsigned char text[], int size);
private:

	std::string printCharArray(unsigned char text[], int size);
	void readHeader(const header_p &meta, FILE * inputFile, FILE * xorOutput, FILE * xorEncryptedOutput,
										  FILE * rsaOutput, FILE * rsaEncryptedOutput, FILE* aesOutput, FILE* aesEncryptedOutput, FILE * sineOutput);
};