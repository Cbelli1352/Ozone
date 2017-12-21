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
int main() {
	http::SocketServer server (5000);
	for (;;) {
		std::string input;
		std::getline(std::cin, input);
		std::transform(input.begin(), input.end(), input.begin(), easytolower);
		if (input == "kill") return 0;
		if (input == "print") {
			continue;
		}
		std::cout << "Invalid command, type \"help\" for help" << std::endl;
	}
}
