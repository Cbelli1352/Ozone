#include "SocketServer.h"

using namespace http;

http::SocketServer::SocketServer(int port) :
		logger(Logger::GetLogger()) {
	int serverSock, clientSock;
	sockaddr_in echoserver, echoclient;
	if ((serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		exit(1);
 	}

	echoserver = { 0 };

 	echoserver.sin_family = AF_INET; /* Internet/IP */
 	echoserver.sin_addr.s_addr = htonl(INADDR_ANY); /* Incoming addr */
 	echoserver.sin_port = htons(port); /* server port */

	logger.Log("Socket Server Started");

	if (bind(serverSock, (struct sockaddr *) &echoserver,
 		sizeof(echoserver)) < 0) {
		logger.LogError("Error creating server");
 		exit(1);
 	}



 	/* Listen on the server socket */
 	if (listen(serverSock, MaxPending) < 0) {
		exit(1);
 	}
	ServerSocket = serverSock;



	Handler = new std::thread (&http::SocketServer::Handle,
				   this, 
				   echoclient,
				   serverSock, 
				   clientSock);
	Handler->detach();
}

void http::SocketServer::Handle(sockaddr_in echoclient, int serversock, int clientsock) {
	
	for(;;) {
 		unsigned int clientlen = sizeof(echoclient);
 		if ((clientsock = accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
			continue;	
 		}	
		logger.Log("Error connecting " + (std::string) std::strerror(errno));
 		inet_ntoa(echoclient.sin_addr);
 		Sockets.push_back(new HttpSocket(clientsock));
 }
	std::cout << "Socket server stopped";
}

void SocketServer::HandleDeath() {
	delete Handler;
	close(ServerSocket);
}
