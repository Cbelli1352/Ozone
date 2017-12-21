#pragma once

#include <thread>
#include <string>
#include <map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <fstream>

#define MaxPending 5
#define BufferSize 4096

namespace http {
	class HttpSocket {
	public:
		HttpSocket(int socket);

		struct HttpHeaders {
			std::string Method;
			std::map<std::string, std::string> Headers;
			std::string Content;
			std::string Path;
		};
	private:
		HttpHeaders ParseHttp(std::string request);

		std::string GetFile(std::string path);

		void HandleRequest();

		std::thread Helper;

		int Socket;
	};
}