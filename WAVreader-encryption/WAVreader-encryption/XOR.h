#pragma once
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <random>
#include <limits>

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

	keyLength encryptXOR(keyLength* buffer, int nrBytesRead)
	{
		int128_t a;
		for (int i = 0; i < nrBytesRead; i++)
		{
			/*int128_t a = boost::lexical_cast<int128_t, long long> (buffer[i]);
			a = a ^ cipher;*/
		    buffer[i] = buffer[i] ^ this->cipher;
			//cout << a << endl;
		}
		return a;
	}

	keyLength encryptXorWav(uint128_t& combinedBuffer)
	{
		//cout << "before cipher: " << combinedBuffer << endl;
		combinedBuffer = combinedBuffer ^ this->cipher;
		//cout << "after cipher: " << combinedBuffer << "\n\n\n";
		combinedBuffer = combinedBuffer ^ this->cipher;
		//cout << "after DECIPHER: " << combinedBuffer << "\n\n\n";
		return combinedBuffer;
	}

	keyLength generateRandomNumber()
	{
		boost::random::mt19937 gen( std::time(0) );
		boost::random::uniform_int_distribution<uint128_t> dist(1, std::numeric_limits<uint128_t>::max());
		/*std::random_device rand_dev;
		std::mt19937 generator(rand_dev());
		std::uniform_int_distribution<long long>  distr(3, INT64_MAX);
		return distr(gen);*/
		return dist(gen);
	}

	keyLength getCipher()
	{
		return cipher;
	}
private:

	keyLength cipher;
};

//#include "XORtemplate.cpp"