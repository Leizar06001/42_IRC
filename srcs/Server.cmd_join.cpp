#include "../includes/server.class.hpp"

void Server::cmd_join(int fd, vector<string> tokens){
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;
	if (tokens.size() < 2){		// no channel
		sendServerMessage(fd, ERR_NEEDMOREPARAMS, " :Need more params");
		return ;
	}


	for(size_t i = 1; i < tokens.size(); ++i){	// FOR EACH CHANNEL IN TOKENS
		int ret = 0;
		_channels->joinChannel(user, tokens[i]);

		switch (ret){
			case ERR_TOOMANYCHANNELS:
				sendServerMessage(fd, ret, "JOIN :Too many channels"); break;
			case ERR_BADCHANMASK:
				sendServerMessage(fd, ret, "JOIN :Bad channel name, dont forget the prefix"); break;
			case ERR_BANNEDFROMCHAN:
				sendServerMessage(fd, ret, "JOIN :You are banned from this chnnel"); break;
			case ERR_CHANNELISFULL:
				sendServerMessage(fd, ret, "JOIN :This channel is full"); break;
			case ERR_INVITEONLYCHAN:
				sendServerMessage(fd, ret, "JOIN :You must be invited to join this channel"); break;


			case 0:
				sendMessage(fd, ":" + user->getNickname() + "!" + user->getUsername() + "@" + user->getIpAdress() + " " +  "JOIN " + tokens[i]);
				string user_mode = _channels->getUserModes(user, tokens[i]);
				s_Channel* chan = _channels->getChannel(tokens[i]);
				if (chan){
					sendRawMsgToList(fd, ":" + user->getNickname() + "!" + user->getUsername() + "@" + _servername + " " +  "JOIN " + tokens[i], chan->users);
					sendRawMsgToList(fd, ":" + _servername + " MODE " + tokens[i] + user_mode + " " + user->getNickname(), chan->users);
				}
				// SEND ALL USERS OF THE CHANNEL TO THE NEW
				vector<string> toks;
				toks.push_back("NAMES");
				toks.push_back(tokens[1]);
				toks.push_back(_channels->getUsersNicksInChan(tokens[i]));
				cmd_names(fd, toks);
				break;
		}
	}
}
