#include <cryptlib.h>
#include <filters.h>
#include <base64.h>
#include <hex.h>
#include <aes.h>
#include <sha.h>

std::string encode ( std::string& s )
{
	std::string encoded; 
	CryptoPP::StringSource(s, true, new CryptoPP::Base64Encoder(new 
	CryptoPP::StringSink(encoded))); 
	return (encoded);
}

std::string decode ( std::string& s )
{
	std::string decoded; 
	CryptoPP::StringSource(s, true, new CryptoPP::Base64Decoder(new 
	CryptoPP::StringSink(decoded))); 
	return (decoded);
}

std::string encrypt ( std::string& s )
{
	//Encrypt the data

	//Return the encoded data base64 encoded (Some encrypted chars can get messed up otherwise)
	return (encode(s));
}

std::string decrypt ( std::string& s )
{
	s = decode(s);
	//Now decrypt the input

	return (s);
}

void generateRandomString(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

std::string string2Hash( std::string varSaltPrefix, std::string varString, std::string varSaltPostfix  )
{
	std::string varDigest;
	CryptoPP::SHA256 hash;

	CryptoPP::StringSource foo(varSaltPrefix << varString << varSaltPostfix, true,
		new CryptoPP::HashFilter(hash,
			new CryptoPP::Base64Encoder (
				new CryptoPP::StringSink(varDigest)
			)
		)
	);

	return varDigest;
}

bool checkHash2Password ( std::string& varHash, std::string& varSaltSent, std::string& varSaltRecieved,  std::string& varPassword)
{
	std::cout << " Salt Sent: " << varSaltSent << " Salt Recieved: " << varSaltRecieved << " Hash: " << varHash << " Result: " << string2Hash(varSaltSent + varPassword + varSaltRecieved);

	if(string2Hash(varSaltSent + varPassword + varSaltRecieved) == varHash){
		return true;
	}
	
	return false;
}
