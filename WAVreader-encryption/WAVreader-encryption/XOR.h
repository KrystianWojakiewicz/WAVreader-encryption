#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <limits>
#include <string>

using namespace boost::multiprecision;

 template <class keyLength> class XOR
{
public:
	XOR()
	{
		this->cipher = generateRandomNumber();
	}
	XOR(keyLength key)
	{
		this->cipher = key;
	}
	~XOR() {}

	std::vector<keyLength> encryptXOR(std::vector<keyLength>& buffer)
	{
		for (int i = 0; i < buffer.size(); i++)
		{
		    buffer[i] = buffer[i] ^ this->cipher;
		}
		return buffer;
	}

	std::string encryptXorWav(std::string& combinedBuffer)
	{
		std::string a = boost::lexical_cast<std::string>(this->cipher);

		for (int i = 0; i < combinedBuffer.size() && i < a.size(); i++)
		{
			combinedBuffer[i] = combinedBuffer[i] ^ a[i];
		}
		
		return combinedBuffer;
	}

	keyLength generateRandomNumber()
	{
		boost::random::mt19937 gen( std::time(0) );
		boost::random::uniform_int_distribution<keyLength> dist(1, std::numeric_limits<keyLength>::max());
		
		return dist(gen);
	}

	keyLength getCipher()
	{
		return cipher;
	}
private:

	keyLength cipher;
};
