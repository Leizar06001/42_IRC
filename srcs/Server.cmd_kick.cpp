#include "../includes/server.class.hpp"

void Server::cmd_kick(int fd, vector<string> tokens){
	if (tokens.size() < 3){
		sendServerMessage(fd, ERR_NEEDMOREPARAMS, "KICK :Target or channel missing");
		return;
	}
	userInfos* kicker = _users->getUserByFd(fd);
	if (!kicker) return;
	userInfos* user = _users->getUserByNick(tokens[2]);
	if (!user){
		sendServerMessage(fd, ERR_NOSUCHNICK, "KICK :User not found");
		return ;
	}

	int ret = _channels->kickChannel(kicker, user, tokens[1]);
	switch (ret){
		case ERR_NOSUCHCHANNEL:
			sendServerMessage(fd, ERR_NOSUCHCHANNEL, "KICK :No such channel");
			break;
		case ERR_CHANOPRIVSNEEDED:
			sendServerMessage(fd, ERR_CHANOPRIVSNEEDED, "KICK :You don't have the priviledges for this action");
			break;
		case ERR_USERNOTINCHANNEL:
			sendServerMessage(fd, ERR_USERNOTINCHANNEL, "KICK :The targeted user is not in this channel");
			break;
		case 0:
			sendClientMessage(user->getFd(), "KICK " + tokens[1] + " " + tokens[2]);
			sendServerMsgToList(fd, "KICK " + tokens[1] + " " + tokens[2] + " :" + kicker->getNickname(), _channels->getChannel(tokens[1])->users);
			break;
	}
}
