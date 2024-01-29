#include "../includes/server.class.hpp"
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <poll.h>
#include <sys/time.h>

Server::Server(void):_initialized(0){};
Server::~Server(void){
	this->shutdown();
};
Server::Server(Server &rhs){(void)rhs;};
Server &Server::operator=(Server &rhs){
	(void)rhs;
	return *this;
}

void Server::shutdown(void){
	std::cout << std::endl << timestamp() << " ------ END SERVER ------" << std::endl;
	for(int i = 1; i <= MAX_CON; ++i){
		if (_fds[i].fd >= 0){
			std::cout << timestamp() << " Closing Connection.. " << i << std::endl;
			close(_fds[i].fd);
		}
	}
	if (_sockfd){
		std::cout << timestamp() << " Closing Socket.." << std::endl;
		close(_sockfd);
	}
	std::cout << timestamp() << " ------ GOODBYE ------" << std::endl;
}

void Server::init(int port){
	std::cout << std::endl << timestamp() << " ------ STARTING SERVER ------" << std::endl;
	if (port <= 0 || port > 65535){
		std::cout << timestamp() << "Error: PORT " << port << " not valid" << std::endl;
		return ;
	}

	_port = port;
	_msg_nb = 0;
	_connection_nb = 0;
	for(int i = 0; i <= MAX_CON; ++i) _fds[i].fd = -1;
	createSocket(AF_INET, SOCK_STREAM, 0);
	bindToPort(_port);
	_initialized = 1;
	std::cout << timestamp() << " ------ INIT DONE ------" << std::endl;
}

void Server::createSocket(int domain, int type, int protocol){
	std::cout << timestamp() << " >>> Creating Socket.." << std::endl;

	_sockfd = socket(domain, type, protocol);
	if (_sockfd == -1) {
		std::cout << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	// SO_REUSEADDR allow to reuse the port if still in use after exiting the program
	// (avoid error like 'cant bind socket to port xxxx')
	const int enable = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		std::cout << "setsockopt(SO_REUSEADDR) failed" << std::endl;

	_fds[0].fd = _sockfd;
	_fds[0].events = POLLIN;

	std::cout << timestamp() << " > ..ok" << std::endl;
}

void Server::bindToPort(int port){
	std::cout << timestamp() << " >>> Binding to port " << port << ".." << std::endl;

	_sockaddr.sin_family = AF_INET;
	_sockaddr.sin_addr.s_addr = INADDR_ANY;
	_sockaddr.sin_port = htons(port); // htons is necessary to convert a number to
									// network byte order
	if (bind(_sockfd, (struct sockaddr*)&_sockaddr, sizeof(sockaddr)) < 0) {
		std::cout << "Failed to bind to port " << port << ". errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(_sockfd, 10) < 0) {
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << timestamp() << " > ..ok " << std::endl;
}

void Server::getConnection(void){
	std::cout << timestamp() << " >>> New Connection.." << std::endl;

	if (_connection_nb >= MAX_CON){
		std::cout << "ERROR: TOO MANY CONNEXIONS" << std::endl;
		return ;
	}
	int	connection;
	unsigned long addrlen = sizeof(sockaddr);
	connection = accept(_sockfd, (struct sockaddr*)&_sockaddr, (socklen_t*)&addrlen);
	if (connection < 0) {
		std::cout << "Failed to grab connection. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	++_connection_nb;
	int i = 1;
	while (_fds[i].fd > 0) ++i;
	_fds[i].fd = connection;
	_fds[i].events = POLLIN;// | POLLHUP;

	std::cout << timestamp() << " > Connection No. " << i << " | " << _connection_nb << " / " << MAX_CON << " clients" << std::endl;
}

int Server::pollFds(int timeout){
	int ret = poll(_fds, _connection_nb + 1, timeout);
	if (ret == -1){
		std::cerr << "Poll error. errno: " << errno << std::endl;
	}
	return ret;
}

void Server::handleEvents(void){
	if (!_initialized){
		std::cout << timestamp() << " SERVER NOT INITIALIZED" << std::endl;
		return ;
	}
	if (pollFds(500) > 0){
		// Check for connection
		if (_fds[0].revents & POLLIN){
			this->getConnection();
		}
		// Check for new messages
		for(int i = 1; i <= _connection_nb; ++i){
			if (_fds[i].fd != -1){
				if (_fds[i].revents & POLLIN){
					this->getMessages(i);
				}
			}
		}
	} else {
		// std::cout << timestamp() << " ...." << std::endl;
	}
}

void Server::parseMessage(std::string& msg, int fd){
	if (msg == "CAP LS 302"){
		// Send a message to the connection
		std::string response = "CAP * LS :\r\n";
		send(_fds[fd].fd, response.c_str(), response.size(), 0);
	}
}

void Server::getMessages(int fd){
	char buffer[10000];
	ssize_t bytesRead = read(_fds[fd].fd, buffer, sizeof(buffer));

	if (bytesRead == 0){	// CLIENT DISCONNECTED
		std::cout << timestamp() << " X Client " << fd << " has disconnected" << std::endl;
		_fds[fd].fd = -1;
		--_connection_nb;
	} else {
		std::string answer(buffer, bytesRead);
		std::string	msg;
		int	 		ok = 0;
		while (answer.length() && !ok){
			size_t pos = answer.find('\r', 0);
			if (pos <= 0) ok = 1;
			if (answer[pos + 1] == '\n'){
				msg = answer.substr(0, pos);
				this->parseMessage(msg, fd);
				std::cout << timestamp() << " >> Client id." << fd << ": Msg No " << _msg_nb << ": " << msg << std::endl;
				answer = answer.substr(pos + 2);
				++_msg_nb;
			}
		}
	}
}





std::string Server::timestamp(void){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	std::string ret = std::to_string(tv.tv_sec % 10000) + "." + std::to_string(tv.tv_usec);
	return ret;
}
