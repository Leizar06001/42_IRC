#include "../includes/server.class.hpp"

void Server::cmd_notice(int fd, vector<string> tokens){
	if (tokens.size() < 3) {
		sendServerMessage(fd, ERR_NEEDMOREPARAMS, ":Need more params");
		return ;
	}
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;
	userInfos* target = _users->getUserByNick(tokens[1]);
	if (!target){
		sendServerMessage(fd, ERR_NOSUCHNICK, tokens[1] + " :No such nickname");
		return ;
	}
	sendMessage(fd, ":" + _servername + " NOTICE " + user->getNickname() + " :" + tokens[2]);
}
