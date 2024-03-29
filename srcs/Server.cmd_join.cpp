#include "../includes/server.class.hpp"
#include <sstream>

void Server::cmd_join(int fd, vector<string> tokens){
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;
	if (tokens.size() < 2){		// no channel
		sendServerMessage(fd, ERR_NEEDMOREPARAMS, " :Need more params");
		return ;
	}
	string key = "";
	if (tokens.size() > 2){
		key = tokens[2];
	}

	std::istringstream ss(tokens[1]);
	std::string channel;
	while (std::getline(ss, channel, ',')) {
		int ret = _channels->joinChannel(user, channel, key);

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
			case ERR_INPUTTOOLONG:
				sendServerMessage(fd, ret, "JOIN :Channel name max 15 characters"); break;
			case ERR_BADCHANNELKEY:
				sendServerMessage(fd, ret, "JOIN :Wrong channel key"); break;

			case 0:
				sendMessage(fd, ":" + user->getNickname() + "!" + user->getUsername() + "@" + user->getIpAdress() + " " +  "JOIN " + channel);
				string user_mode = _channels->getUserModes(user, channel);
				s_Channel* chan = _channels->getChannel(channel);
				if (chan){
					sendRawMsgToList(fd, ":" + user->getNickname() + "!" + user->getUsername() + "@" + _servername + " " +  "JOIN " + channel, chan->users);
					if (user_mode.find_first_of("Aao") != string::npos)
						sendRawMsgToList(fd, ":" + _servername + " MODE " + channel + " " + user_mode + " " + user->getNickname() + " " + user->getNickname(), chan->users);
				}
				// SEND ALL USERS OF THE CHANNEL TO THE NEW
				vector<string> toks;
				toks.push_back("NAMES");
				toks.push_back(channel);
				toks.push_back(_channels->getUsersNicksInChan(channel));
				cmd_names(fd, toks);
				sendServerMessage(fd, RPL_TOPIC, channel + " :" + chan->topic);
				break;
		}
	}
}
