#include <cryptlib.h>
#include <filters.h>
#include <base64.h>
#include <hex.h>
#include <sha.h>
#include <modes.h>
#include <aes.h>
#include <ccm.h>
#include <assert.h>
#include <misc.h>
#include <osrng.h>
#include <crc.h>

std::string generateRandomString(std::string& varResult, const int len)
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
    	"_=+()*&$@!{}[]:%";

    srand ( time(NULL) );
    varResult = "";
    for (int i = 0; i < len; ++i) {
    	varResult = varResult + alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return varResult;
}

byte* generatePRNB( byte* varInput, const int len)
{
	static const char alphanum[] =
	        "0123456789";
	        //"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	        //"abcdefghijklmnopqrstuvwxyz";
	    	//"_=+()*&$@!{}[]:%";

	srand ( time(NULL) );

	for (int i = 0; i < len; ++i) {
		varInput = varInput + alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	return varInput;
}

std::string encode ( std::string& s )
{
	std::string encoded;

	CryptoPP::StringSource(s, true, new CryptoPP::Base64Encoder(new 
	CryptoPP::StringSink(encoded)));

	 std::cout << " : Encoded text returned: " << encoded << std::endl;

	return (encoded);
}

std::string decode ( std::string& s )
{
	std::cout << "Encoded text in: " << s;

	std::string decoded; 
	CryptoPP::StringSource(s, true, new CryptoPP::Base64Decoder(new 
	CryptoPP::StringSink(decoded))); 
	return (decoded);
}

std::string encrypt ( std::string& varInput )
{
	//Encrypt the data
	CryptoPP::AutoSeededRandomPool prng;

	byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	//prng.GenerateBlock(key, sizeof(key));
	generatePRNB(key, sizeof(key));

	byte iv[CryptoPP::AES::BLOCKSIZE];
	generatePRNB(iv, sizeof(iv));
	//prng.GenerateBlock(iv, sizeof(iv));

	std::string cipher, encoded, recovered;

	try
	{
	    std::cout << "Plain text in: " << varInput << " - Using IV:" << iv << " key " << key;

	    CryptoPP::CBC_Mode< CryptoPP::AES >::Encryption e;
	    e.SetKeyWithIV( key, sizeof(key), iv );

	    // The StreamTransformationFilter adds padding
	    //  as required. ECB and CBC Mode must be padded
	    //  to the block size of the cipher.
	    CryptoPP::StringSource( varInput, true,
	        new CryptoPP::StreamTransformationFilter( e,
	            new CryptoPP::StringSink( cipher )
	        ) // StreamTransformationFilter
	    ); // StringSource
	}
	catch( CryptoPP::Exception& e )
	{
	    std::cerr << e.what() << std::endl;
	    cipher = "ERROR!";
	}

	//Return the encoded data base64 encoded (Some encrypted chars can get messed up otherwise)
	return (encode(cipher));
}

std::string decrypt ( std::string& s )
{
	s = decode(s);
	//Now decrypt the input

	try
	{
		CryptoPP::AutoSeededRandomPool prng;
		std::string cipher, encoded, recovered;

		byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
		prng.GenerateBlock(key, sizeof(key));

		byte iv[CryptoPP::AES::BLOCKSIZE];
		prng.GenerateBlock(iv, sizeof(iv));

		CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption d;
	    d.SetKeyWithIV( key, sizeof(key), iv );

	    // The StreamTransformationFilter removes
	    //  padding as required.
	    CryptoPP::StringSource( cipher, true,
	        new CryptoPP::StreamTransformationFilter( d,
	            new CryptoPP::StringSink( recovered )
	        ) // StreamTransformationFilter
	    ); // StringSource

	    std::cout << " : Recovered text out: " << recovered << std::endl;
	}
	catch( CryptoPP::Exception& e )
	{
	    std::cerr << e.what() << std::endl;
	    s = "ERROR!";
	}

	return (s);
}

inline std::string trimString(const std::string& Src, const std::string& c = " \r\n")
{
	int p2 = Src.find_last_not_of(c);
	if (p2 == std::string::npos) return std::string();
	int p1 = Src.find_first_not_of(c);
	if (p1 == std::string::npos) p1 = 0;
	return Src.substr(p1, (p2-p1)+1);
}

void log( std::string varLog )
{
	std::cout << varLog << std::endl;
}

std::string string2Hash( std::string varSaltPrefix, std::string varString, std::string varSaltPostfix  )
{
	//log("Hash the following: " + varSaltPrefix + varString + varSaltPostfix);
	std::string varDigest;
	CryptoPP::SHA256 hash;

	CryptoPP::StringSource foo(varSaltPrefix + varString + varSaltPostfix, true,
		new CryptoPP::HashFilter(hash,
			new CryptoPP::Base64Encoder (
				new CryptoPP::StringSink(varDigest)
			)
		)
	);

	return trimString(varDigest);
}

bool checkHash2Password ( std::string& varHash, std::string& varSaltServer, std::string& varSaltClient,  std::string& varPassword)
{

	std::string varCheck;
	varCheck = string2Hash(varSaltServer, varPassword, varSaltClient);
	if(varCheck == varHash){
		return true;
	}
	
	log("Expected: " + varCheck + " Recieved: " + varHash);
	return false;
}

void process ( void )
{

}
