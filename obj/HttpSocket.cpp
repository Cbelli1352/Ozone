#include "HttpSocket.h"

http::HttpSocket::HttpSocket(int socket) {
	std::cout << "Request accepted" << std::endl;
	Socket = socket;
	Helper = new std::thread(&http::HttpSocket::HandleRequest, this);
	Helper->detach();
}

void http::HttpSocket::HandleRequest() {
	int recieved = -1;
	while (recieved != 0) {
		std::array<char, BufferSize> buff = { 0 };
		
		if ((recieved = recv(Socket, buff.data(), BufferSize, 0)) < 0) {
			perror("Error Recieving Data");
			exit(1);
		}
		if (recieved < 1) break;
		std::string request (buff.data(), BufferSize);

		HttpHeaders headers = ParseHttp(request);

		std::string content = "<html><head></head><body> Your useragent is "+
							   headers.Headers["User-Agent"]+"</body></html>";
		std::string response = "HTTP/1.1 200 OK\n"
							   "Content-Type: text/html\n"
							   "Content-Length: " + std::to_string(content.size()) +
							   "\n\n" + content;
		int temp = send(Socket, response.data(), response.size(), 0);
	}

	std::cout << "Request finished" << std::endl;
}

http::HttpSocket::HttpHeaders http::HttpSocket::ParseHttp(std::string request) {
	std::istringstream requestStream (request);

	HttpHeaders parsedHeaders;

	std::string line;

	std::string method;

	std::getline(requestStream, line);

	for (unsigned int i = 0; i < line.size(); ++i) {
		if (line[i] != ' ') {
			method += line[i];
		} else  {
			line = line.substr(i+1, line.size());
			parsedHeaders.Method = method;
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

	if (std::getline(requestStream, line)) {
		parsedHeaders.Content = line;
	}

	return parsedHeaders;
}