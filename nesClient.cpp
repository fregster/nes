#include <stdio.h>
#include <iostream>
#include <string>
#include "ClientSocket.h"
#include "SocketException.h"
#include "Transform.cpp"

int main ( int argc, char* argv[] )
{
	if (argc < 4) {
		// Check the value of argc. If not enough parameters have been passed, inform user and exit.
		std::cout << "Usage is -s <scope> -p <password> -e <encrypt|decrypt>\n"; // Inform the user of how to use the program
		std::cin.get();
		exit(0);
	} else {
		try
			{
				ClientSocket client_socket ( "localhost", 30000 );

				std::string varReply;
				std::string varSaltServer;
				std::string varSaltClient;
				std::string varPassword;
				std::string varScope("scope");
				std::string varEncrypt("e");

				char* varSaltClientc;

				for (int i = 1; i < argc; i++)
				{
					/*
					if (i + 1 != argc)
					{
						switch (argv[i])
						{
							case '-s':
								varScope = argv[i + 1];
							break;
							case '-p':
								varPassword = argv[i + 1];
							break;
							case '-e':
								varEncrypt = argv[i + 1];
							break;
						}
					}
					*/

					if (i + 1 != argc)
					{
						std::cout << argv[i] << ":" << argv[i +1] << std::endl;
						if (argv[i] == "-s") {
							std::cout << "-s matched " << argv[i + 1];
							varScope = std::string(argv[i + 1]);
						} else if (argv[i] == "-p") {
							std::cout << "-p matched " << argv[i + 1];
							varPassword = std::string(argv[i + 1]);
						} else if (argv[i] == "-e") {
							std::cout << "-e matched " << argv[i + 1];
							varEncrypt = std::string(argv[i + 1]);
						} else {
							exit;
						}
					}

				}

				try
				{
					std::string varInput((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());

					generateRandomString(varSaltClient, 24);
					varPassword = "Password123!";

					//std::cout << varSaltClient;

					client_socket >> varSaltServer;

					std::string toSend;
					toSend = "-" + varEncrypt + "-" + varScope + "-" + varSaltClient + "-" + string2Hash(varSaltServer, varPassword, varSaltClient) + "--";

					std::cout << "Recieved Salt: " << varSaltServer << " -- Sending Connection String: " <<  toSend << std::endl;

					client_socket << toSend + varInput;

					client_socket >> varReply;
				}
				catch ( SocketException& ) {}

				std::cout << "We received this response from the server:\n\"" << varReply << "\"\n";;

			}
			catch ( SocketException& e )
			{
				std::cout << "An exception was caught:" << e.description() << std::endl;
			}
	}

  return 0;
}
