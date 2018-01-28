#include "HttpSocket.h"

using namespace http;

int HttpSocket::ThreadNum = 0;

HttpSocket::HttpSocket(int socket) 
	: Helper (&http::HttpSocket::HandleRequest, this),
	logger(Logger::GetLogger()) 
{
	logger.Log("Socket Started");
	ThreadNum++;
	Socket = socket;
	Helper.detach();
}

void HttpSocket::HandleRequest() {
	int recieved = 1;
	while (recieved > 0) {
		std::string buff;
		size_t i = 0;
		do {
			buff.resize(BufferSize);
			if ((recieved = recv(Socket, (char *) buff.data()+i*BufferSize, BufferSize, 0)) < 0) {
				logger.Log("Connection reset by peer. Id: " + (std::string) std::strerror(errno));
				break;
			}
			++i;
		} while (buff[buff.size()] != '\0');
		if (recieved <= 1) break;
		std::string request (buff.data(), BufferSize);

		try {
			logger.Log("Parsing Headers");

			HttpHeaders headers = ParseHttp(request);
			
			logger.Log("Getting file " + headers.Path);

			std::string response = GetFile(headers.Path);

			send(Socket, response.data(), response.size(), 0);
		} catch (std::exception& e) {
			logger.LogError("Invalid request");
			close(Socket);
			break;
		}
	}
	ThreadNum--;
	logger.Log("Request finished code " + std::to_string(ThreadNum));
}

std::string HttpSocket::GetFile (std::string path) {
	if (path[path.size()-1] == '/') {
		path += "index.html";
	}
	std::ifstream file("." + path);

	if (!file.is_open()) {
		logger.LogError("File " + path + " Not found");	
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
	file.read((char *) buffer.data(), size); 

	std::string response = "HTTP/1.1 200 OK\n"
							"Content-Type: text/"+ fileEnding+"\n"
							"Content-Length: " + std::to_string(size) +
							"\n\n" + buffer;
	logger.Log("Responded with " + path);
	return response;
}

HttpSocket::HttpHeaders HttpSocket::ParseHttp(std::string request) {
	std::istringstream requestStream (request);

	HttpHeaders parsedHeaders;

	std::string line;

	std::getline(requestStream, line);

	for (unsigned int i = 0; i < line.size(); ++i) {
		if (line[i] == ' ') {
			parsedHeaders.Method = line.substr(0, i);
			line = line.substr(i+1, line.size());
			break;
		}
	}


	for (unsigned int i = 0; i < line.size(); ++i) {
		if (line[i] == ' ') {
			parsedHeaders.Path = line.substr(0, i);
		}
	}

	if (parsedHeaders.Path == "") throw std::exception();


	while (std::getline(requestStream, line)) {
		if (line.size() == 0) break;

		std::string header, value;

		for (unsigned int i = 0; i < line.size(); ++i) {
			if (line[i] == ':') {
				header = line.substr(0, i);
				value = line.substr(i+2, line.size());
				break;
			}
		}

		parsedHeaders.Headers[header] = value;
	}

	if (std::getline(requestStream, line)) parsedHeaders.Content = line;

	return parsedHeaders;
}
