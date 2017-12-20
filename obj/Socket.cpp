#include "Socket.h"

void http::Socket::Die(std::string e) {
	perror(e.c_str());
	exit(1);
}

http::Socket::Socket(int socket) {
	std::cout << "Socket accepted" << std::endl;
	_Socket = socket;
	Handler = new std::thread(&http::Socket::Handle, this);
	Handler->detach();
	State = 1;
}	

void http::Socket::Handle() {
	// Number of bytes recieved. Keeps the loop alive until closed
	int recieved = -1;
	
	// Loops and collects data from socket
	while (recieved != 0) {
		// Makes buffer 
		std::array<char, BufferSize> buff = { 0 };
		
		if ((recieved = recv(_Socket, buff.data(), BufferSize, 0)) < 0) {
			Die("Error Recieving Data");
		}
		std::cout << "Recieved" << std::endl;
		Data.push(buff);
		std::string content = "HTTP/1.1 200 OK\n"
							   "Content-Length: 65\n"
							   "Content-Type: text/html\n\n"
							   "<!DOCTYPE html> <html> <head> </head> <body> test </body> </html>\n";
		std::cout << "Sending Data" << std::endl;
		int temp = send(_Socket, content.c_str(), content.size(), 0);
		std::cout << "Sent Data" << std::endl;
	}
	// Set the state to closed
	State = 2;
}

std::array<char, BufferSize> http::Socket::GetLastBuffer() {
	if (Data.size()) {
		std::array<char, BufferSize> data = Data.front();
		Data.pop();
		return data;
	} else return { 0 };
}

void http::Socket::Send(std::array<char, BufferSize> buffer) {
	
}