#include <string>
#include <iostream>
#include "ServerSocket.h"
#include "SocketException.h"
#include "Transform.cpp"

int main ( int argc, char* argv[] )
{
	std::cout << "running....\n";

	try
	{
		// Create the socket
		ServerSocket server ( 30000 );

		while ( true )
		{

			ServerSocket new_sock;
			server.accept ( new_sock );

			try
			{
				while ( true )
				{
					bool varEncrypt (true);
					size_t varFound(0);
					std::string varScope;
					std::string varHash;
					char* varSaltSentc;
					std::string varSaltRecieved;
					std::string varPassword;
				  	std::string data;
					std::string key;

					//Standard start buildup
					//Sent Salt1 which is used as part of the password hashing
					//-e-scope-hash-salt--input
					//sha256(salt+password+salt)

					//Generate and end the salt
					generateRandomString(varSaltSentc, 24);
					std::string varSaltSent(varSaltSentc);
					new_sock << varSaltSent;

					//Get and then process Data
					new_sock >> data;

					//Check if encrypting or decrypting
					if(data.substr(0,2) == "-d" ) {
						varEncrypt = false;
						data.erase (0,int(varFound)+2);
					}

					//Check scope (Key set)
					varFound = data.find("-");
		  			if (varFound != std::string::npos) {
						varScope = data.substr (0,int(varFound));
		    				data.erase (0,int(varFound)+1);
					}

					//Get Hash and Salt to verify connection
					varFound = data.find("-");
		  			if (varFound != std::string::npos) {
		  				varSaltRecieved = data.substr (0,int(varFound));
		    				data.erase (0,int(varFound)+1);
					}

					varFound = data.find("-");
		  			if (varFound != std::string::npos) {
						varHash = data.substr (0,int(varFound));
		    				data.erase (0,int(varFound)+1);
					}

					//Verify the connection
		  			varPassword = "Password123!";
					if(checkHash2Password(varHash, varSaltSent, varSaltRecieved, varPassword)) {
						//Find termination string and drop everything before it
			  			varFound = data.find("--");
			  			if (varFound != std::string::npos) {
			    				data.erase (0,int(varFound)+2);
						}

						if(varEncrypt){
							//Decrypt
							new_sock << encrypt(data);
						} else {
							//Encrypt
							new_sock << decrypt(data);
						}
					}

					new_sock << "Verification Failed";
				}
			}
			catch ( SocketException& ) {}
		}
	}
	catch ( SocketException& e )
	{
		std::cout << "An exception was caught:" << e.description() << "\nExiting.\n";
	}

	return 0;
}
