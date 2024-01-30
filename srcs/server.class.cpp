#include "../includes/server.class.hpp"
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream>
#include <unistd.h> // For read
#include <poll.h>
#include <cerrno>
#include "../includes/toString.hpp"

using namespace std;

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
	cout << endl;
	_term.prtTmColor("------ END SERVER ------\n", Terminal::CYAN);
	for(int i = 1; i <= MAX_CON; ++i){
		if (_fds[i].fd >= 0){
			_term.prtTmColor("Closing Connection.. " + toString(i) + "\n", Terminal::RESET);
			close(_fds[i].fd);
		}
	}
	if (_sockfd){
		_term.prtTmColor("Closing Socket..\n", Terminal::RESET);
		close(_sockfd);
	}
	_term.prtTmColor("------ GOODBYE ------\n", Terminal::CYAN);
}

void Server::drawInterface(void){
	static int dec = 1;
	int	h = 10;
	_term.saveCursor();
	for(int i = 1; i <= h + 1; ++i){
		_term.clearLine(i);
	}
	_term.setCursor(3, dec);
	_term.prtColor(">>    >>    >>    >>    >>    >>    >>    >>    >>    >>    >>    >>    >>", Terminal::BLUE);
	for(int i = 1; i <= h; ++i){
		_term.setCursor(i, 1);
		_term.prtColor("*", Terminal::BLUE);
		_term.setCursor(i, 80);
		_term.prtColor("*", Terminal::BLUE);
		_term.clearLineAfter();
	}
	_term.setCursor(1, 1);
	_term.prtColor("********************************************************************************", Terminal::BLUE);
	_term.setCursor(h, 1);
	_term.prtColor("********************************************************************************", Terminal::BLUE);

	// TITLE
	_term.setCursor(3, 25);
	_term.prtColor(" (ﾟ◥益◤ﾟ) İ͍͔R̲̟͍̯ͬĈ̝̙̻̬͊̒ͪ ̳̹͎̂̈́̏ͪS̏̅ͮ͆ͩEŔ̲̪̓V̙̘ͭE͕̺̝ͣ̇ͧͯͅR̗̽͌̍ͯͪ  (ʘ言ʘ╬) ", Terminal::CYAN);
	// LEFT SIDE
	_term.setCursor(5, 5);
	_term.prtColor("   Port: " + toString(_port), Terminal::CYAN);
	_term.setCursor(6, 5);
	_term.prtColor("Clients: " + toString(_connection_nb) + "/" + toString(MAX_CON), Terminal::GREEN);
	int not_reg = _users.getNbNotRegistered();
	if (not_reg) {
		_term.setCursor(6, 22);
		_term.prtColor("! " + toString(not_reg) + " not reg", Terminal::RED);
	}
	// RIGHT SIDE
	_term.setCursor(6, 54);
	_term.prtColor("Channels: " + toString(0), Terminal::MAGENTA);
	_term.setCursor(7, 54);
	_term.prtColor("Messages: " + toString(_msg_nb), Terminal::MAGENTA);

	_term.restoreCursor();
	cout << flush;	// update display
	++dec;
	if (dec > 6) dec = 1;
}

int Server::init(int port){
	_port = port;
	_msg_nb = 0;
	_connection_nb = 0;
	_sockfd = 0;
	for(int i = 0; i <= MAX_CON; ++i) _fds[i].fd = -1;

	_term.clearScreen();
	drawInterface();
	_term.setCursor(12, 1);
	_term.prtTmColor("------ STARTING SERVER ------\n", Terminal::CYAN);
	if (port <= 0 || port > 65535){
		_term.prtTmColor("Error: PORT " + toString(port) + " not valid\n", Terminal::RED);
		return -1;
	}
	createSocket(AF_INET, SOCK_STREAM, 0);
	bindToPort(_port);
	_initialized = 1;
	_term.prtTmColor("------ INIT DONE ------\n", Terminal::CYAN);
	return 0;
}

void Server::createSocket(int domain, int type, int protocol){
	_term.prtTmColor(" >>> Creating Socket..\n", Terminal::RESET);

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

	_term.prtTmColor(" > ..ok\n", Terminal::RESET);
}

void Server::bindToPort(int port){
	_term.prtTmColor(">>> Binding to port " + toString(port) + "..\n", Terminal::RESET);

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

	_term.prtTmColor(" > ..ok\n", Terminal::RESET);
}

void Server::getConnection(void){
	_term.prtTmColor(">>> New Connection..\n", Terminal::GREEN);

	if (_connection_nb >= MAX_CON){
		_term.prtTmColor("ERROR: TOO MANY CONNEXIONS\n", Terminal::RED);
		return ;
	}
	int	connection;
	unsigned long addrlen = sizeof(sockaddr);
	connection = accept(_sockfd, (struct sockaddr*)&_sockaddr, (socklen_t*)&addrlen);
	if (connection < 0) {
		_term.prtTmColor("Failed to grab connection. errno: " + toString(errno) + "\n", Terminal::RED);
		exit(EXIT_FAILURE);
	}

	++_connection_nb;
	int i = 1;
	while (_fds[i].fd > 0) ++i;
	_fds[i].fd = connection;
	_fds[i].events = POLLIN;// | POLLHUP;

	_users.addUser(i);

	_term.prtTmColor(">>> Connection FD." + toString(i), Terminal::GREEN);
	_term.prtColor(" | " + toString(_connection_nb) + " / " + toString(MAX_CON) + " clients\n", Terminal::CYAN);
}

int Server::pollFds(int timeout){
	int ret = poll(_fds, MAX_CON, timeout);
	if (ret == -1){
		_term.prtTmColor("Poll error. errno: " + toString(errno) + "\n", Terminal::RED);
	}
	return ret;
}

void Server::handleEvents(void){
	if (!_initialized){
		_term.prtTmColor("SERVER NOT INITIALIZED\n", Terminal::RED);
		return ;
	}
	//drawInterface();
	if (pollFds(500) > 0){
		// Check for connection
		if (_fds[0].revents & POLLIN){
			this->getConnection();
		}
		// Check for new messages
		int i = 0;
		int fd = 1;
		while (i < _connection_nb){
			if (_fds[fd].fd != -1){
				if (_fds[fd].revents & POLLIN){
					this->getMessages(fd);
				}
				++i;
			}
			++fd;
		}
	} else {
		// _term.prtTmColor(" ....", Terminal::RESET);
	}
	drawInterface();
}

void Server::parseMessage(string& msg, int fd){
	vector<string> 	tokens;
	string 			token;

	// Split the message at ':' if exists
	size_t	colonPos = msg.find(" :");
    string	beforeColon = (colonPos != string::npos) ? msg.substr(0, colonPos) : msg;
    string	afterColon = (colonPos != string::npos) ? msg.substr(colonPos + 1) : "";

	// Split at spaces
	stringstream 	ss(beforeColon);
	while (std::getline(ss, token, ' ')) {
		if (!token.empty()) {
			tokens.push_back(token);
		}
	}
	// Push back after ':' part
	if (!afterColon.empty()) tokens.push_back(afterColon);

	_term.prtTmColor("CMD: '" + tokens[0] + "'\n", Terminal::YELLOW);

	if (tokens[0] == "NICK"){
		_users.setNickname(fd, tokens[1]);
	}
	if (tokens[0] == "USER"){
		if (tokens[1].length() > 1)
			_users.setUsername(fd, tokens[1]);
	}
	if (msg == "CAP LS 302"){
		sendMessage(fd, string("CAP * LS :\r\n"));
	}


	if (tokens[0] == "MAPID"){
		_users.getUserByFd(atoi(tokens[1].c_str()));
	}
	if (tokens[0] == "RMID"){
		_users.rmUser(atoi(tokens[1].c_str()));
	}

}

void Server::getMessages(int fd){
	// cout << "---> Reading Fd " << fd, Terminal::RESET);
	char buffer[10000];
	ssize_t bytesRead = read(_fds[fd].fd, buffer, sizeof(buffer));

	if (bytesRead == 0){	// CLIENT DISCONNECTED
		_term.prtTmColor("X Client # " + toString(fd) + " has disconnected\n", Terminal::RED);
		_users.rmUser(fd);
		_fds[fd].fd = -1;
		--_connection_nb;
	} else {				// TREAT MESSAGE
		string answer(buffer, bytesRead);
		size_t pos;
		while ((pos = answer.find("\r\n", 0)) != string::npos){
			string msg = answer.substr(0, pos);
			userInfos* user = _users.getUserByFd(fd);
			_term.prtTmColor(">> " + user->getNickname() + ", Msg #" + toString(user->getNbMsg()) + ": '" + toString(msg) + "'\n", Terminal::RESET);
			this->parseMessage(msg, fd);
			answer = answer.substr(pos + 2);
			++_msg_nb;
			// cout << "---> Reading again " << fd, Terminal::RESET);
		}
	}
	// cout << "---> Reading END " << fd, Terminal::RESET);
}

void Server::sendMessage(int fd, const string& msg){
	send(_fds[fd].fd, msg.c_str(), msg.size(), 0);
	_term.prtTmColor("sending: '" + msg + "' to fd " + toString(fd) + "\n", Terminal::RESET);
}
