# Makefile for the Network Encryption Service
#

CXX = g++
CXXFLAGS = -lcrypto++ -I. -I./libs/crypto
OUTPUT = -o build/
LOG = 2> build/error.log

nes_server_objects = ServerSocket.o Socket.o nes_server.o
nes_client_objects = ClientSocket.o Socket.o nes_client.o


all : server client

server: $(nes_server_objects)
	$(CXX) $(OUTPUT)server_nes $(nes_server_objects) $(CXXFLAGS) $(LOG)


client: $(nes_client_objects)
	$(CXX) $(OUTPUT)client_nes $(nes_client_objects) $(CXXFLAGS) $(LOG)


Socket: Socket.cpp
ServerSocket: ServerSocket.cpp
ClientSocket: ClientSocket.cpp
nes_server: nes_server.cpp
nes_client: nes_client.cpp


clean:
	rm -f *.o build/server_nes build/client_nes build/error.log
