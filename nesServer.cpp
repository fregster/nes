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

			ServerSocket varServerSocket;
			server.accept ( varServerSocket );

			try
			{
				while ( true )
				{
					bool varEncrypt (true);
					size_t varFound(0);
					std::string varScope;
					std::string varHash;
					std::string varSaltServer;
					std::string varSaltClient;
					std::string varPassword;
					std::string varData;
					std::string key;


					//Standard ensure the vars are clean
					varScope.clear();
					varHash.clear();
					varSaltClient.clear();
					varSaltServer.clear();
					varPassword.clear();
					varData.clear();
					key.clear();


					//Standard start buildup
					//Sent Salt1 which is used as part of the password hashing
					//-e-scope-hash-salt--input
					//sha256(salt+password+salt)

					//Generate and end the salt
					generateRandomString(varSaltServer, 24);
					varServerSocket << varSaltServer;


					//Get and then process Data
					varServerSocket >> varData;

					//log(varData);

					//Check if encrypting or decrypting
					if(varData.substr(0,2) == "-d" ) {
						varEncrypt = false;
					}
					varData.erase (0,int(varFound)+3);

					//Check scope (Key set)
					varFound = varData.find("-");
					if (varFound != std::string::npos) {
						varScope = varData.substr (0,int(varFound));
						varData.erase (0,int(varFound)+1);
					}

					//Get Hash and Salt to verify connection
					varFound = varData.find("-");
					if (varFound != std::string::npos) {
						varSaltClient = varData.substr (0,int(varFound));
						varData.erase (0,int(varFound)+1);
					}

					varFound = varData.find("-");
					if (varFound != std::string::npos) {
						varHash = varData.substr (0,int(varFound));
						varData.erase (0,int(varFound));
					}

					//Find termination string and drop everything before it
					varFound = varData.find("--");
					if (varFound != std::string::npos) {
						varData.erase (0,int(varFound)+2);
					}

					//Load the configurations for the scope
					varPassword = "Password123!";

					//Verify the connection
					//std::string& varHash, std::string& varSaltServer, std::string& varSaltClient,  std::string& varPassword
					if(checkHash2Password(varHash, varSaltServer, varSaltClient, varPassword)) {
						if(varEncrypt){
							//Decrypt
							varServerSocket << encrypt(varData);
						} else {
							//Encrypt
							varServerSocket << decrypt(varData);
						}
					}

					//varServerSocket << "Scope: " << varScope << " - Hash: " << varHash << " - Sent: " <<  varSaltServer << " - Recieved: " <<  varSaltClient << " - Password: " <<  varPassword;
					varServerSocket << "Verification Failed" << "\n\n";
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
