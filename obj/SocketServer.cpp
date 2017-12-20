#include "SocketServer.h"

http::SocketServer::SocketServer(int port) {
	int serverSock, clientSock;
	sockaddr_in echoserver, echoclient;
	if ((serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
 	}
	echoserver = { 0 };
 	echoserver.sin_family = AF_INET; /* Internet/IP */
 	echoserver.sin_addr.s_addr = htonl(INADDR_ANY); /* Incoming addr */
 	echoserver.sin_port = htons(port); /* server port */
	std::cout << "Handler Started" << std::endl;
	if (bind(serverSock, (struct sockaddr *) &echoserver,
 		sizeof(echoserver)) < 0) {
 		
 	}
 	/* Listen on the server socket */
 	if (listen(serverSock, MaxPending) < 0) {
 	}
	Handler = new std::thread (&http::SocketServer::Handle, this, echoclient, serverSock, clientSock);
	Handler->detach();
}

void http::SocketServer::Handle(sockaddr_in echoclient, int serversock, int clientsock) {
	std::cout << "Socket server listening" << std::endl;
	for(;;) {
 		unsigned int clientlen = sizeof(echoclient);
 		if ((clientsock = accept(serversock, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
			continue;
 		}
 		inet_ntoa(echoclient.sin_addr);
 		Sockets.push_back(new Socket(clientsock));
 }
	std::cout << "Socket server stopped";
}