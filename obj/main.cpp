#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include "SocketServer.h"
#include <algorithm>

char easytolower(char in){
  if(in<='Z' && in>='A')
    return in-('Z'-'z');
  return in;
} 

void printSocketData(const http::SocketServer & server) {
	for (auto& socket : server.Sockets) {
		std::array<	char, BufferSize> data = socket->GetLastBuffer();
		std::string string ((char *) data.data(), data.size());
		std::cout << string << " |" << std::endl;
	}
	std::cout << std::endl;
}

int main() {
	http::SocketServer server (5000);
	for (;;) {
		std::string input;
		std::getline(std::cin, input);
		std::transform(input.begin(), input.end(), input.begin(), easytolower);
		if (input == "kill") return 0;
		if (input == "print") {
			printSocketData(server);
			continue;
		}
		std::cout << "Invalid command, type \"help\" for help" << std::endl;
	}
}
