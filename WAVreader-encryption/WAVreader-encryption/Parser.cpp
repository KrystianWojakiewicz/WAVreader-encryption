#include "Parser.h"


Parser::Parser(std::string filepath)
{
	header_p meta = (header_p)malloc(sizeof(header));							// header_p points to a header struct that contains the wave file metadata fields
								
	xorInput = fopen(&filepath[0], "rb");
	xorOutput = fopen("xorOutput.wav", "wb");
	xorEncryptedOutput = fopen("xorEncryptedOutput.wav", "wb");

	rsaInput = fopen(&filepath[0], "rb");
	rsaOutput = fopen("rsaOutput.wav", "wb");
	rsaEncryptedOutput = fopen("rsaEncryptedOutput.wav", "wb");
	readHeader(meta, rsaInput, xorOutput, xorEncryptedOutput, rsaOutput, rsaEncryptedOutput);
}

Parser::~Parser()
{
	std::fclose(rsaInput);
	std::fclose(xorInput);

	std::fclose(xorOutput);
	std::fclose(xorEncryptedOutput);
	std::fclose(rsaOutput);
	std::fclose(rsaEncryptedOutput);
}

std::string Parser::printCharArray(unsigned char text[], int size)
{
	std::string convertedText;
	for (int i = 0; i < size; i++)
	{
		convertedText += text[i];
	}
	return convertedText;
}

unsigned int Parser::changeEndianness(unsigned char text[], int size)
{
	unsigned int convertedText;
	if (size == 4)
	{
		convertedText = text[0] |
			(text[1] << 8) |
			(text[2] << 16) |
			(text[3] << 24);
	}
	else if (size == 2)
	{
		convertedText = text[0] |
			(text[1] << 8);
	}

	return convertedText;
}

void Parser::readHeader(const header_p &meta, FILE * rsaInput, FILE * xorOutput, FILE * xorEncryptedOutput, FILE * rsaOutput, FILE * rsaEncryptedOutput)
{
	std::fread(meta, 1, sizeof(header), rsaInput);
	std::fwrite(meta, 1, sizeof(*meta), xorOutput);
	std::fwrite(meta, 1, sizeof(*meta), xorEncryptedOutput);
	std::fwrite(meta, 1, sizeof(*meta), rsaOutput);
	std::fwrite(meta, 1, sizeof(*meta), rsaEncryptedOutput);

	std::cout << " Chunk ID: " << printCharArray(meta->chunk_id, 4) << std::endl;
	std::cout << " Chunk Size: " << changeEndianness(meta->chunk_size, 4) << " bytes " << std::endl;
	std::cout << " Chunk Format: " << printCharArray(meta->format, 4) << std::endl;

	std::cout << " SubChunk_1 ID: " << printCharArray(meta->subchunk1_id, 4) << std::endl;
	std::cout << " SubChunk_1 Size: " << changeEndianness(meta->subchunk1_size, 4) << " bytes " << std::endl;
	std::cout << " SubChunk_1 Format: " << changeEndianness(meta->audio_format, 2) << std::endl;
	std::cout << " Number of Channels: " << changeEndianness(meta->num_channels, 2) << std::endl;
	std::cout << " Sample Rate: " << changeEndianness(meta->sample_rate, 4) << " Hz" << std::endl;
	std::cout << " Byte Rate: " << changeEndianness(meta->byte_rate, 4) << std::endl;
	std::cout << " Block Allign: " << changeEndianness(meta->sample_rate, 2) << std::endl;
	std::cout << " Bits per Sample: " << changeEndianness(meta->bits_per_sample, 4) << " bits" << std::endl;


	std::cout << " SubChunk_2 ID: " << printCharArray(meta->subchunk2_id, 4) << std::endl;
	std::cout << " SubChunk_2 Size: " << changeEndianness(meta->subchunk2_size, 4) << " bytes" << std::endl;
}