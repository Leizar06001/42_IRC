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
			_channels->quitServer(_users->getUserByFd(fd));
			//rmUser(fd, string("QUIT Weird messages"));
			forceDisconnect(fd, "Weird");
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

		userInfos* user = _users->getUserByFd(fd);
		if (!user) return;
		answer = user->getIncommingMsg() + answer;	// RETRIEVE PREVIOUS FRAGMENTED DATAS
		size_t pos = 0;
		size_t start = 0;
		while ((pos = answer.find("\n", start)) != string::npos){	// FIND MESSAGES IN DATAS RECEIVED
			size_t msgLen = pos - start;
			if (pos > 0 && answer[pos - 1] == '\r')
				--msgLen;
			string msg = answer.substr(start, msgLen);

			if (!msg.empty()){
				user = _users->getUserByFd(fd);
				if (user){
					user->incMsgs();
					vector<string> tokens = parseMessage(fd, msg);	// PARSE
					analyseCommands(fd, tokens);					// LAUNCH CMDS
				}
			}

			start = pos + 1;
			++_msg_nb;
		}
		user = _users->getUserByFd(fd);
		if (!user) return;
		if (start < answer.length()){		// ADD FRAGMENTED DATA
			if (answer.substr(start).length() > 1000){
				user->incWrongCmds();
				_term.prtTmColor(user->getNickname() + " has " + toString(user->getWrongCmdsNb()) + " wrong commands", Terminal::RED);
			} else {
				if (user) user->setIncommingMsg(answer.substr(start));
				_term.prtTmColor("FD." + toString(fd) + " Fragmented message: " + answer.substr(start), Terminal::BRIGHT_YELLOW);
			}
		} else {
			if (user) user->setIncommingMsg("");
		}
	}
}

void Server::sendMessage(int fd, const string& msg){
	string final_msg = msg + "\r\n";
	int ret = send(_fds[fd].fd, final_msg.c_str(), final_msg.size(), 0);
	(void)ret;
	if (ret == -1) _term.prtTmColor("ERROR SENDING MESSAGE\n", Terminal::RED);
	else _term.prtTmColor("OUT: '" + Terminal::MAGENTA + msg + "'" + Terminal::YELLOW + " -> " + toString(fd) + "\n", Terminal::BRIGHT_MAGENTA);
}

void Server::sendClientMessage(int fd, const string& msg){
	userInfos* dest = _users->getUserByFd(fd);
	const string final_msg = ":" + dest->getNickname() + "!" + dest->getUsername() + "@" + _servername + " " + msg;
	const string msg_to_send = final_msg + "\r\n";

	int ret = send(_fds[fd].fd, msg_to_send.c_str(), msg_to_send.size(), 0);
	(void)ret;
	if (ret == -1) _term.prtTmColor("ERROR SENDING MESSAGE " + final_msg, Terminal::RED);
	else _term.prtTmColor("OUT: '" + Terminal::MAGENTA + final_msg + "'" + Terminal::YELLOW + " -> " + toString(fd) + "\n", Terminal::BRIGHT_MAGENTA);
}

void Server::sendClientMessageShowIp(int fd, const string& msg){
	userInfos* dest = _users->getUserByFd(fd);
	const string msg_to_print = ":" + Terminal::YELLOW + dest->getNickname() + Terminal::BRIGHT_MAGENTA + "!" + dest->getUsername() + "@" + dest->getIpAdress() + " " + msg;
	const string msg_to_send = ":" + dest->getNickname() + "!" + dest->getUsername() + "@" + dest->getIpAdress() + " " + msg + "\r\n";

	int ret = send(_fds[fd].fd, msg_to_send.c_str(), msg_to_send.size(), 0);
	(void)ret;
	if (ret == -1) _term.prtTmColor("ERROR SENDING MESSAGE " + msg_to_print, Terminal::RED);
	else _term.prtTmColor("OUT: '" + Terminal::BRIGHT_MAGENTA + msg_to_print + "'" + Terminal::YELLOW + " -> " + toString(fd) + "\n", Terminal::BRIGHT_MAGENTA);
}

void Server::sendServerMessage(int fd, int rpl_err_code, const string& msg){
	userInfos* dest = _users->getUserByFd(fd);
	const string nick = (dest == NULL) ? "" : dest->getNickname();
	const string msg_to_print = ":" + _servername + " " + toString(rpl_err_code) + " " + Terminal::YELLOW + nick + Terminal::MAGENTA + " " + msg;
	const string msg_to_send = ":" + _servername + " " + toString(rpl_err_code) + " " + nick + " " + msg + "\r\n";

	int ret = send(_fds[fd].fd, msg_to_send.c_str(), msg_to_send.size(), 0);
	(void)ret;
	if (ret == -1) _term.prtTmColor("ERROR SENDING MESSAGE " + msg_to_print, Terminal::RED);
	else _term.prtTmColor("OUT: '" + Terminal::MAGENTA + msg_to_print + "'" + Terminal::YELLOW + " -> " + toString(fd) + "\n", Terminal::BRIGHT_MAGENTA);
}

void	Server::sendMsgToList(int fd_source, const string& msg, const map<int, size_t> &lst){
	userInfos* source = _users->getUserByFd(fd_source);
	map<int, size_t>::const_iterator it = lst.begin();
	if (msg.length() == 0 || !source) return;
	while (it != lst.end()){
		if (it->first != fd_source){
			sendMessage(it->first, ":" + source->getNickname() + "!" + source->getUsername() + "@" + _servername + " " + msg);
		}
		++it;
	}
}

void	Server::sendMsgToList(int fd_source, const string& msg, vector<userInfos*> lst){
	userInfos* source = _users->getUserByFd(fd_source);
	(void)lst;
	vector<userInfos*>::iterator it = lst.begin();
	(void)it;
	if (msg.length() == 0 || !source) return;
	while (it != lst.end()){
		if ((*it)->getFd() != fd_source){
			sendMessage((*it)->getFd(), ":" + source->getNickname() + "!" + source->getUsername() + "@" + _servername + " " + msg);
		}
		++it;
	}
}

void	Server::sendServerMsgToList(int fd_source, const string& msg, vector<userInfos*> lst){
	userInfos* source = _users->getUserByFd(fd_source);
	(void)lst;
	vector<userInfos*>::iterator it = lst.begin();
	(void)it;
	if (msg.length() == 0 || !source) return;
	while (it != lst.end()){
		if ((*it)->getFd() != fd_source){
			sendMessage((*it)->getFd(), ":" + _servername + " " + msg);
		}
		++it;
	}
}
