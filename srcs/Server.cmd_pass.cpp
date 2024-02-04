#include "../includes/server.class.hpp"

void	Server::cmd_pass(int fd, vector<string> tokens){
	if (tokens.size() < 2){
		sendMessage(fd, ":" + _servername + " " + toString(ERR_NEEDMOREPARAMS) + " :Password is empty");
		rmUser(fd, string("Empty password"));
		return ;
	}
	if (tokens[1] == _password){
		userInfos* user = _users->getUserByFd(fd);
		if (!user) return;
		user->setPasswordOk();
	} else {
		sendMessage(fd, ":" + _servername + " " + toString(ERR_PASSWDMISMATCH) + " :Wrong password");
		rmUser(fd, string("Wrong password"));
	}
}
