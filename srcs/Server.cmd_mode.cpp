#include "../includes/server.class.hpp"

void Server::cmd_mode(int fd, vector<string> tokens){
	if (tokens.size() < 2) return;
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;
	if (tokens[1][0] == '#'){		// target is a channel
		s_Channel* chan = _channels->getChannel(tokens[1]);
		if (chan == NULL){ // channel not exists
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NOSUCHCHANNEL) + " " + user->getNickname() + " :" + tokens[1]);
		} else {
			sendMessage(fd, ":" + _servername + " " + toString(RPL_CHANNELMODEIS) + " " + user->getNickname() + " " + tokens[1] + " " + chan->mode);
		}
	} else {						// target is a user

	}
}
