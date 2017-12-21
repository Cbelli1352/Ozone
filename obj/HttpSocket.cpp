#include "HttpSocket.h"

using namespace http;

HttpSocket::HttpSocket(int socket) 
	: Helper (&http::HttpSocket::HandleRequest, this) {
	std::cout << "Request accepted" << std::endl;
	Socket = socket;
	Helper.detach();
}

void HttpSocket::HandleRequest() {
	int recieved = -1;
	while (recieved != 0) {
		std::string buff;
		size_t i = 0;
		do {
			buff.resize(BufferSize);
			if ((recieved = recv(Socket, (char *) buff.data()+i*BufferSize, BufferSize, 0)) < 0) {
				std::cout << "Connection reset by peer" << std::endl;
				return;
			}
			++i;
		} while (buff[buff.size()] != '\0');
		if (recieved < 1) break;
		std::string request (buff.data(), BufferSize);

		HttpHeaders headers = ParseHttp(request);

		std::string response = GetFile(headers.Path);

		int temp = send(Socket, response.data(), response.size(), 0);
	}

	std::cout << "Request finished" << std::endl;
}

std::string HttpSocket::GetFile (std::string path) {
	if (path[path.size()-1] == '/') {
		path += "index.html";
	}
	std::ifstream file("./html" + path);

	if (!file.is_open()) {
		std::cout << "File not found" << std::endl;
		return "HTTP/1.1 404 Not Found\nContent-Type: text/plain\nContent-Length: 9\n\nNot Found";
	}

	std::string fileEnding;

	for (unsigned int i = path.size(); i > 0; --i) {
		if (path[i] == '.') {
			fileEnding = path.substr(i+1, path.size());
			break;	
		}
	}

	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	std::string buffer(size, ' ');
	file.seekg(0);
	file.read(&buffer[0], size); 

	std::string response = "HTTP/1.1 200 OK\n"
							"Content-Type: text/"+ fileEnding+"\n"
							"Content-Length: " + std::to_string(size) +
							"\n\n" + buffer;
	return response;
}

HttpSocket::HttpHeaders HttpSocket::ParseHttp(std::string request) {
	std::istringstream requestStream (request);

	HttpHeaders parsedHeaders;

	std::string line;

	std::getline(requestStream, line);

	for (unsigned int i = 0; i < line.size(); ++i) {
		if (line[i] != ' ') {
			parsedHeaders.Method += line[i];
		} else  {
			line = line.substr(i+1, line.size());
			break;
		}
	}


	for (unsigned int i = 0; i < line.size(); ++i) {
		if (line[i] != ' ') {
			parsedHeaders.Path += line[i];
		} else break;
	}


	while (std::getline(requestStream, line)) {
		if (line.size() == 0) break;

		std::string header, value;

		for (unsigned int i = 0; i < line.size(); ++i) {
			if (line[i] != ':') {
				header += line[i];
			} else {
				value = line.substr(i+2, line.size());
				break;
			}
		}

		parsedHeaders.Headers[header] = value;
	}

	if (std::getline(requestStream, line)) parsedHeaders.Content = line;

	return parsedHeaders;
}