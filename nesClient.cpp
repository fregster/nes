#include <stdio.h>
#include <iostream>
#include <string>
#include "ClientSocket.h"
#include "SocketException.h"
#include "Transform.cpp"

int main ( int argc, char* argv[] )
{
	try
	{
		ClientSocket client_socket ( "localhost", 30000 );

		std::string reply;
		char* varSaltRecieved;
		char* varSaltSent;

		try
		{
			std::string varInput((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());

			varSaltSent = generateRandomString(varSaltSent, 16)

			client_socket >> varSaltRecieved;

			client_socket << varInput;

			client_socket >> reply;
		}
		catch ( SocketException& ) {}

		std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;

	}
	catch ( SocketException& e )
	{
		std::cout << "An exception was caught:" << e.description() << std::endl;
	}

  return 0;
}
