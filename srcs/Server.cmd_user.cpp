#include "../includes/server.class.hpp"

void Server::cmd_user(int fd, vector<string> tokens){
	if (_users->getUserByFd(fd)->isRegistered()) {
		// SEND ERROR ALREADY REGISTERED
	} else {
		if (tokens.size() < 5){
			// INFOS MISSING
			return ;
		}
		if (tokens[1].length() > 1){
			int ret = _users->setUsername(fd, tokens[1]);
			if (!ret)
				ret = _users->setRealname(fd, tokens[4]);
			if (!ret){
				_users->checkForRegistration(fd);
			} else {
				// ERROR USER
			}
		}
	}
}
