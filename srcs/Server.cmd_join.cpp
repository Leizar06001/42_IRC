#include "../includes/server.class.hpp"

void Server::cmd_join(int fd, vector<string> tokens){
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;
	if (tokens.size() < 2){		// no channel
		sendServerMessage(fd, ERR_NEEDMOREPARAMS, " :Need more params");
	} else {
		for(size_t i = 1; i < tokens.size(); ++i){	// FOR EACH CHANNEL IN TOKENS
			int ret = 0;
			_channels->joinChannel(user, tokens[i]);
			if (ret == 0){								// CHANNEL JOINED
				sendClientMessage(fd, "JOIN " + tokens[i]);
				s_Channel* chan = _channels->getChannel(tokens[i]);
				if (chan){
					sendMsgToList(fd, "JOIN " + tokens[i], chan->users);
				}
				// SEND ALL USERS OF THE CHANNEL TO THE NEW
				// vector<string> toks;
				// toks.push_back("NAMES");
				// toks.push_back(tokens[1]);
				// toks.push_back(_channels->getUsersNicksInChan(tokens[1]));
				// cmd_names(fd, toks);


			} else if (ret == ERR_NOSUCHCHANNEL){
				sendServerMessage(fd, ERR_NOSUCHCHANNEL, tokens[i] + " :No such channel");
			} else if (ret == ERR_NOTONCHANNEL){
				sendServerMessage(fd, ERR_NOTONCHANNEL, tokens[i] + " :Not on channel");
			}
		}
	}



}
