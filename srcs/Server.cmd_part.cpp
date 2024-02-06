#include "../includes/server.class.hpp"

void Server::cmd_part(int fd, vector<string> tokens){
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;
	if (tokens.size() < 2){		// no channel
		sendServerMessage(fd, ERR_NEEDMOREPARAMS, " :Need more params");
	} else {
		for(size_t i = 1; i < tokens.size(); ++i){
			int ret = 0;
			_channels->partChannel(user, tokens[i]);
			if (ret == 0){
				sendClientMessage(fd, "PART " + tokens[i]);
				s_Channel* chan = _channels->getChannel(tokens[i]);
				if (chan)
					sendMsgToList(fd, "PART " + tokens[i], chan->users);
			} else if (ret == ERR_NOSUCHCHANNEL){
				sendServerMessage(fd, ERR_NOSUCHCHANNEL, tokens[i] + " :No such channel");
			} else if (ret == ERR_NOTONCHANNEL){
				sendServerMessage(fd, ERR_NOTONCHANNEL, tokens[i] + " :Not on channel");
			}
		}
	}
}
