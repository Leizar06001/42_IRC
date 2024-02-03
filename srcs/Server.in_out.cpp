#include "../includes/server.class.hpp"
#include <cstring>

int Server::pollFds(int timeout){
	int ret = poll(_fds, _max_clients, timeout);
	if (ret == -1){
		_term.prtTmColor("Poll error: " + toString(strerror(errno)) + "\n", Terminal::RED);
	}
	return ret;
}

void Server::readSockets(void){
	if (pollFds(250) > 0){
		// Check for new clients
		if (_fds[0].revents & POLLIN){
			this->getConnection();
		} else {
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
		}
	}
}

void Server::getMessages(int fd){
	char buffer[1000] = {0};
	ssize_t bytesRead = recv(_fds[fd].fd, buffer, sizeof(buffer), MSG_DONTWAIT);


	if (bytesRead < 0 ){
		// ERROR
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			_term.prtTmColor("RECV ERROR: " + toString(strerror(errno)), Terminal::BRIGHT_RED);
			_channels->leaveServer(_users->getUserByFd(fd));
			rmUser(fd, string("QUIT Weird messages"));
		}
        // If EAGAIN or EWOULDBLOCK, simply no data available now, not an error
	} else if (bytesRead == 0){	// CLIENT DISCONNECTED
		forceDisconnect(fd, "Connection lost");
	} else {				// TREAT MESSAGE
		string answer(buffer, bytesRead);
		if (isBotTraffic(answer)){					// BOT DETECTION
			userInfos* bot = _users->getUserByFd(fd);
			if (bot) addToBannedList(bot->getIpAdress());
			forceDisconnect(fd, "BANNED");
			return;
		}
		size_t pos = 0;
		size_t start = 0;
		while ((pos = answer.find("\n", start)) != string::npos){	// FIND MESSAGES IN DATAS RECEIVED
			size_t msgLen = pos - start;
			if (pos > 0 && answer[pos - 1] == '\r')
				--msgLen;
			string msg = answer.substr(start, msgLen);

			if (!msg.empty()){
				userInfos* user = _users->getUserByFd(fd);
				if (user){
					user->incMsgs();
					vector<string> tokens = parseMessage(fd, msg);	// PARSE
					analyseCommands(fd, tokens);					// LAUNCH CMDS
					if (user->getWrongCmdsNb() > 2){
						_channels->leaveServer(user);
						rmUser(fd, string("QUIT Too many wrong messages"));
					}
				}
			}

			start = pos + 1;
			++_msg_nb;
		}
	}
}

void Server::sendMessage(int fd, const string& msg){
	string final_msg = msg + "\r\n";
	int ret = send(_fds[fd].fd, final_msg.c_str(), final_msg.size(), 0);
	if (ret == -1) _term.prtTmColor("ERROR SENDING MESSAGE\n", Terminal::RED);
	else _term.prtTmColor("OUT: '" + msg + "' to fd " + toString(fd) + "\n", Terminal::BRIGHT_MAGENTA);
}
// sendMessage(fd, ":" + nick + "!" + user->getUsername() + "@" + _servername + " MODE " + nick + " " + target->getUserMode());

void Server::sendClientMessage(int fd, const string& msg){
	userInfos* dest = _users->getUserByFd(fd);
	const string final_msg = ":" + dest->getNickname() + "!" + dest->getUsername() + "@" + _servername + " " + msg;
	const string msg_to_send = final_msg + "\r\n";

	int ret = send(_fds[fd].fd, msg_to_send.c_str(), msg_to_send.size(), 0);
	if (ret == -1) _term.prtTmColor("ERROR SENDING MESSAGE " + final_msg, Terminal::RED);
	else _term.prtTmColor("OUT: '" + final_msg + "' to fd " + toString(fd) + "\n", Terminal::BRIGHT_MAGENTA);
}

void Server::sendClientMessageShowIp(int fd, const string& msg){
	userInfos* dest = _users->getUserByFd(fd);
	const string final_msg = ":" + dest->getNickname() + "!" + dest->getUsername() + "@" + dest->getIpAdress() + " " + msg;
	const string msg_to_send = final_msg + "\r\n";

	int ret = send(_fds[fd].fd, msg_to_send.c_str(), msg_to_send.size(), 0);
	if (ret == -1) _term.prtTmColor("ERROR SENDING MESSAGE " + final_msg, Terminal::RED);
	else _term.prtTmColor("OUT: '" + final_msg + "' to fd " + toString(fd) + "\n", Terminal::BRIGHT_MAGENTA);
}

void Server::sendServerMessage(int fd, int rpl_err_code, const string& msg){
	userInfos* dest = _users->getUserByFd(fd);
	const string nick = (dest == NULL) ? "" : dest->getNickname();
	const string final_msg = ":" + _servername + " " + toString(rpl_err_code) + " " + nick + " " + msg;
	const string msg_to_send = final_msg + "\r\n";

	int ret = send(_fds[fd].fd, msg_to_send.c_str(), msg_to_send.size(), 0);
	if (ret == -1) _term.prtTmColor("ERROR SENDING MESSAGE " + final_msg, Terminal::RED);
	else _term.prtTmColor("OUT: '" + final_msg + "' to fd " + toString(fd) + "\n", Terminal::BRIGHT_MAGENTA);
}
