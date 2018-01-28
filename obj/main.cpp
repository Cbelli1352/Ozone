#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include "SocketServer.h"
#include <algorithm>
#include <signal.h>
#include <stdlib.h>

char easytolower(char in){
  if(in<='Z' && in>='A')
    return in-('Z'-'z');
  return in;
}

http::SocketServer server (5000);

void cmdExit(int sig = 0) {
	server.HandleDeath();
	exit(sig);
}


int main() {
	signal (SIGINT, cmdExit);
	std::string input;
	for (;;) {
		std::getline(std::cin, input);
		std::transform(input.begin(), input.end(), input.begin(), easytolower);
		if (input == "exit") {
			cmdExit();
			continue;
		}
		if (input == "print") {
			continue;
		}
		if (input == "help") {
			continue;
		}
		std::cout << "Invalid command, type \"help\" for help" << std::endl;
	}
}
