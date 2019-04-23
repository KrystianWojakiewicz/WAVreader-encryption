#include "Parser.h"


Parser::Parser(std::string filepath)
{
    meta = (header_p)malloc(sizeof(header));							// header_p points to a header struct that contains the wave file metadata fields
								
	inputFile = fopen(&filepath[0], "rb");
	xorOutput = fopen("xorOutput.wav", "wb");
	xorEncryptedOutput = fopen("xorEncryptedOutput.wav", "wb");

	rsaOutput = fopen("rsaOutput.wav", "wb");
	rsaEncryptedOutput = fopen("rsaEncryptedOutput.wav", "wb");

	sineOutput = fopen("sineOutput.wav", "wb");

	readHeader(meta, inputFile, xorOutput, xorEncryptedOutput, rsaOutput, rsaEncryptedOutput, sineOutput); // write header data to outpuFiles 
}

Parser::~Parser()
{
	std::fclose(inputFile);
	
	std::fclose(xorOutput);
	std::fclose(xorEncryptedOutput);
	
	std::fclose(rsaOutput);
	std::fclose(rsaEncryptedOutput);

	delete meta;
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

void Parser::readHeader(const header_p &meta, FILE * rsaInput, FILE * xorOutput, FILE * xorEncryptedOutput, FILE * rsaOutput, FILE * rsaEncryptedOutput, FILE * sineOutput)
{
	std::fread(meta, 1, sizeof(header) -8, rsaInput);
	
	//check for empty spaces after bits_per_sample because the standard allows for them
	char tmp = 0;
	while (tmp != 'd')
	{
		int nb = std::fread(&tmp, 1, 1, rsaInput);
	}

	meta->subchunk2_id[0] = 'd';
	std::fread(&meta->subchunk2_id[1], 1, 3, rsaInput);
	std::fread(meta->subchunk2_size, 1, 4, rsaInput);
	
	//Initialize all output files with header data
	std::fwrite(meta, 1, sizeof(*meta), xorOutput);
	std::fwrite(meta, 1, sizeof(*meta), xorEncryptedOutput);
	std::fwrite(meta, 1, sizeof(*meta), rsaOutput);
	std::fwrite(meta, 1, sizeof(*meta), rsaEncryptedOutput);
	std::fwrite(meta, 1, sizeof(*meta), sineOutput);

	//print header data to console
	std::cout << "----------.WAV HEADER----------" << "\n\n";

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
	std::cout << " Bits per Sample: " << changeEndianness(meta->bits_per_sample, 2) << " bits" << std::endl;


	std::cout << " SubChunk_2 ID: " << printCharArray(meta->subchunk2_id, 4) << std::endl;
	std::cout << " SubChunk_2 Size: " << changeEndianness(meta->subchunk2_size, 4) << " bytes" << "\n\n";
}