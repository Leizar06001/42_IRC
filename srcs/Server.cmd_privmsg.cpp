#include "../includes/server.class.hpp"

void Server::cmd_msg(int fd, vector<string> tokens){
	if (!tokens[1].empty() && !tokens[2].empty()){
		userInfos* dest = _users->getUserByNick(tokens[1]);
		if (dest){
			sendMessage(dest->getFd(), ":" + _users->getUserByFd(fd)->getNickname() + " PRIVMSG " + tokens[1] + " :" + tokens[2]);
		} else {
			// ERR DEST NOT EXISTS
		}
	}
}
