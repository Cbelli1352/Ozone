#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <queue>
#include <thread>
#include <memory>
#include <array>
#include <iostream>

#define MaxPending 5
#define BufferSize 4096

// Wrapper for linux socket

namespace http {
	class Socket {
		public:
			Socket(int socket);
			void Send(std::array<char, BufferSize> data); // Sends buffer to socket
			std::array<char, BufferSize> GetLastBuffer(); // Dequeues data and returns it
		
			int State = 0; // 0: Opening, 1: Open, 2: Closed
		private:

			void Die(std::string e); // Gives error and kills task
			
			int _Socket; // The id of the socket

			void Handle(); // Handles the input on another thread and adds to queue
		
			std::queue<std::array<char, BufferSize>> Data; // List of data
		
			std::thread * Handler; // Thread that handles data
	};
}


