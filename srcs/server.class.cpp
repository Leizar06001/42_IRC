#include "../includes/server.class.hpp"
#include <netinet/in.h>
#include <arpa/inet.h>

Server::Server(void):_initialized(0), _term(&_logStream){
	_servername = "IRis.Chat";
	_users = new userList(&_term, PRINT_DEBUG_INFOS);
	_channels = new ChannelList(&_term);
};
Server::~Server(void){
	delete _users;
	delete _channels;
	this->shutdown();
};
Server::Server(Server &rhs):_term(&_logStream){(void)rhs;};
Server &Server::operator=(Server &rhs){
	(void)rhs;
	return *this;
}

void Server::shutdown(void){
	cout << endl;
	_term.prtTmColor("------ END SERVER ------\n", Terminal::BRIGHT_CYAN);
	for(int i = 1; i <= MAX_CON; ++i){
		if (_fds[i].fd >= 0){
			_term.prtTmColor("Closing Connection.. " + toString(i) + "\n", Terminal::WHITE);
			close(_fds[i].fd);
		}
	}
	if (_sockfd){
		_term.prtTmColor("Closing Socket..\n", Terminal::WHITE);
		close(_sockfd);
	}
	if (_logStream.is_open()) {
		_term.prtTmColor("Closing Log file..\n", Terminal::WHITE);
		_logStream.close();
		return ;
	}

	_term.prtTmColor("-------- GOODBYE -------\n", Terminal::BRIGHT_CYAN);
}

void Server::drawInterface(void){
	_term.updateTitle(_port, _connection_nb, _users->getNbNotRegistered(), _channels->getNbChannel(), _msg_nb, MAX_CON);
	_term.updateMenu(_users, _channels);
}

int Server::init(int port){
	_port = port;
	_msg_nb = 0;
	_connection_nb = 0;
	_sockfd = 0;
	_last_timeout_check = time(NULL);
	for(int i = 0; i <= MAX_CON; ++i){
		_fds[i].fd = -1;
		_fds[i].events = 0;
		_fds[i].revents = 0;
	}

	_term.clearScreen();
	drawInterface();

	// Create LOG folder and open log file
	openLog();
	// Create CONF folder and read conf file if exists
	readConf();

	// INIT SOCKET
	_term.prtTmColor("------ STARTING SERVER ------\n", Terminal::BRIGHT_CYAN);
	if (port <= 0 || port > 65535){
		_term.prtTmColor("Error: PORT " + toString(port) + " not valid\n", Terminal::RED);
	}
	createSocket(AF_INET, SOCK_STREAM, 0);
	bindToPort(_port);
	_initialized = 1;
	_term.prtTmColor("--------- INIT DONE ---------\n", Terminal::BRIGHT_CYAN);
	return 0;
}

void Server::createSocket(int domain, int type, int protocol){
	_term.prtTmColor(">>> Creating Socket..\n", Terminal::WHITE);

	_sockfd = socket(domain, type, protocol);
	if (_sockfd == -1) {
		_term.prtTmColor("Failed to create socket. errno: " + toString(errno) + "\n", Terminal::RED);
		exit(EXIT_FAILURE);
	}
	// SO_REUSEADDR allow to reuse the port if still in use after exiting the program
	// (avoid error like 'cant bind socket to port xxxx')
	const int enable = 1;
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		_term.prtTmColor("setsockopt(SO_REUSEADDR) failed\n", Terminal::RED);

	_fds[0].fd = _sockfd;
	_fds[0].events = POLLIN;

	_term.prtTmColor("> ..ok\n", Terminal::WHITE);
}

void Server::bindToPort(int port){
	_term.prtTmColor(">>> Binding to port " + toString(port) + "..\n", Terminal::WHITE);

	_sockaddr.sin_family = AF_INET;
	_sockaddr.sin_addr.s_addr = INADDR_ANY;
	_sockaddr.sin_port = htons(port); // htons is necessary to convert a number to
									// network byte order
	if (bind(_sockfd, (struct sockaddr*)&_sockaddr, sizeof(sockaddr)) < 0) {
		_term.prtTmColor("Failed to bind to port " + toString(port) + ". errno: " + toString(errno) + "\n", Terminal::RED);
		exit(EXIT_FAILURE);
	}

	if (listen(_sockfd, 10) < 0) {
		_term.prtTmColor("Failed to listen on socket. errno: " + toString(errno) + "\n", Terminal::RED);
		exit(EXIT_FAILURE);
	}

	_term.prtTmColor("> ..ok\n", Terminal::WHITE);
}

void Server::getConnection(void){
	_term.prtTmColor(">>> New Connection..\n", Terminal::GREEN);

	if (_connection_nb >= MAX_CON){
		_term.prtTmColor("ERROR: TOO MANY CONNEXIONS\n", Terminal::RED);
		return ;
	}

	struct sockaddr_in client_addr;
	unsigned long addrlen = sizeof(client_addr);
	int connection = accept(_sockfd, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);
	if (connection < 0) {
		_term.prtTmColor("Failed to grab connection. errno: " + toString(errno) + "\n", Terminal::RED);
		exit(EXIT_FAILURE);
	}

	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

	string ip_str = string(client_ip);
	if (isIPBanned(ip_str)) {
        _term.prtTmColor(">>> Banned IP: " + string(client_ip) + " - Connection refused\n", Terminal::RED);
		string msg =  ":" + _servername + " " + toString(ERR_YOUREBANNEDCREEP) + " " + ip_str + " :You're banned motherfucker !\r\n";
		send(connection, msg.c_str(), msg.size(), 0);
		close(connection); // Close the connection to the banned IP
        return;
    }

	if (_connection_nb >= MAX_CON){
		_term.prtTmColor(">>> Rejected: " + string(client_ip) + " - Too many connections\n", Terminal::RED);
		string msg =  ":" + _servername + " :No room left on the server\r\n";
		send(connection, msg.c_str(), msg.size(), 0);
		close(connection); // Close the connection to the banned IP
		return ;
	}

	++_connection_nb;
	int i = 1;
	while (_fds[i].fd > 0) ++i;
	_fds[i].fd = connection;
	_fds[i].events = POLLIN;// | POLLHUP;

	userInfos* new_user = _users->addUser(i);
	new_user->setIpAddr(ip_str);

	_term.prtTmColor(">>> " + string(client_ip) + " FD." + toString(i) + Terminal::BRIGHT_CYAN + " | " + toString(_connection_nb) + " / " + toString(MAX_CON) + " clients\n", Terminal::GREEN);
}

int	Server::isIPBanned(string& ip){
	size_t i = 0;
	while (i < _bans_ip.size()){
		if (ip == _bans_ip[i])
			return 1;
		++i;
	}
	return 0;
}

void Server::handleEvents(void){
	if (!_initialized){
		_term.prtTmColor("SERVER NOT INITIALIZED\n", Terminal::RED);
		return ;
	}
	readSockets();
	// Check if a user waits for an action from the server
	checkPendingActions();
	// Check for user not registered in time
	checkClientRegistrationTimeout();
	// Check if connection timeout
	checkIfClientsAlive();
	drawInterface();
}

void Server::rmUser(int fd, const string& reason){
	_term.prtTmColor("X Client # " + toString(fd) + " disconnected: " + reason + "\n", Terminal::RED);
	_channels->leaveServer(_users->getUserByFd(fd));
	_users->rmUser(fd);
	close(_fds[fd].fd);
	_fds[fd].fd = -1;
	--_connection_nb;
}
