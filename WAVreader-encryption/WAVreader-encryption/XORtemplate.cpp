//
//#include <random>
//
//
//template<class keyLength> XOR<keyLength>::XOR()
//{
//	this->cipher = generateRandomNumber();
//}
//
//template<class keyLength> XOR<keyLength>::XOR(keyLength key)
//{
//	this->cipher = key;
//}
//
//template<class keyLength> XOR<keyLength>::~XOR() {}
//
//template<class keyLength>short int* XOR<keyLength>::encryptXOR(short int buffer[BUFSIZ])
//{
//	for (int i = 0; i < BUFSIZ; i++)
//	{
//		buffer[i] = cipher;
//	}
//	return buffer;
//}
//
//template<class keyLength>keyLength XOR<keyLength>::generateRandomNumber()
//{
//	std::random_device rand_dev;
//	std::mt19937 generator(rand_dev());
//	std::uniform_int_distribution<long long>  distr(3, INT16_MAX);
//	return distr(generator);
//}
