#pragma once

#include <string>
#include "AESAlgorithm.h"

std::string addSineToWave(int count, int nrBytesRead, std::string sineBuffer, Parser* parser, int signalStartSec, int signalEndSec, int period);

void sampleRSA(RSA<boost::multiprecision::uint1024_t> &rsa);

void sampleXOR(XOR<boost::multiprecision::uint1024_t> &xor);

void sampleAES(AESAlgorithm &aes);
