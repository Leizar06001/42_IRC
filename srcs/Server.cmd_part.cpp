#include "../includes/server.class.hpp"
#include <sstream>

void Server::cmd_part(int fd, vector<string> tokens){
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;
	if (tokens.size() < 2){		// no channel
		sendServerMessage(fd, ERR_NEEDMOREPARAMS, " :Need more params");
	} else {
		istringstream ss(tokens[1]);
		string channel;
		while (getline(ss, channel, ',')){
			int ret = _channels->partChannel(user, channel);
			if (ret == 0){
				sendClientMessage(fd, "PART " + channel);
				s_Channel* chan = _channels->getChannel(channel);
				if (chan)
					sendMsgToList(fd, "PART " + channel, chan->users);
			} else if (ret == ERR_NOSUCHCHANNEL){
				sendServerMessage(fd, ERR_NOSUCHCHANNEL, channel + " :No such channel");
			} else if (ret == ERR_NOTONCHANNEL){
				sendServerMessage(fd, ERR_NOTONCHANNEL, channel + " :Not on channel");
			}
		}
	}
}
