#include "../includes/server.class.hpp"

int Server::pollFds(int timeout){
	int ret = poll(_fds, MAX_CON, timeout);
	if (ret == -1){
		_term.prtTmColor("Poll error. errno: " + toString(errno) + "\n", Terminal::RED);
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
			_term.prtTmColor("RECV ERROR " + toString(strerror(errno)), Terminal::BRIGHT_RED);
			vector<string> tokens;
			tokens.push_back("QUIT");
			tokens.push_back("???");
			cmd_quit(fd, tokens);
		}
        // If EAGAIN or EWOULDBLOCK, simply no data available now, not an error
	} else if (bytesRead == 0){	// CLIENT DISCONNECTED
		vector<string> tokens;
		tokens.push_back("QUIT");
		tokens.push_back("Connection lost");
		cmd_quit(fd, tokens);
	} else if (bytesRead >= 999){
		vector<string> tokens;
		tokens.push_back("QUIT");
		tokens.push_back("Msgs too long !");
		cmd_quit(fd, tokens);
	} else {				// TREAT MESSAGE
		string answer(buffer, bytesRead);
		size_t pos = 0;
		size_t start = 0;
		while ((pos = answer.find("\n", start)) != string::npos){
			size_t msgLen = pos - start;
			if (pos > 0 && answer[pos - 1] == '\r')
				--msgLen;
			string msg = answer.substr(start, msgLen);

			if (!msg.empty()){
				userInfos* user = _users->getUserByFd(fd);
				if (user){
					user->incMsgs();
					vector<string> tokens = parseMessage(fd, msg);
					analyseCommands(fd, tokens);
				}
			}

			start = pos + 1;
			++_msg_nb;
		}
	}
}

void Server::sendMessage(int fd, const string& msg){
	string final_msg = msg + "\r\n";
	send(_fds[fd].fd, final_msg.c_str(), final_msg.size(), 0);
	_term.prtTmColor("OUT: '" + msg + "' to fd " + toString(fd) + "\n", Terminal::BRIGHT_MAGENTA);
}
