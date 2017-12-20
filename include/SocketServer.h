#pragma once

#include "Socket.h"
#include <vector>

// Listens on a port and makes socket instances to handle them
namespace http {
	class SocketServer {
		public:
			SocketServer(int port); // Makes socket factory from port
			std::vector<Socket*> Sockets; // All of the sockets on the server
		private:
			void Handle(sockaddr_in echoclient, int serversock, int clientSock); // Thread that handles socket connections
			std::thread * Handler; // Manages thread
	};
}