#include "../includes/server.class.hpp"

void Server::cmd_msg(int fd, vector<string> tokens){
	int err = 0;
	if (tokens.size() > 1){
		if (tokens.size() > 2){
			// Check if dest is a channel
			userInfos* user = _users->getUserByFd(fd);
			if (!user) return ;
			if (tokens[1][0] == '#'){
				s_Channel* chan = _channels->getChannel(tokens[1]);
				if (chan){		// Channel exists
					if (_channels->is_in_Channel(user, tokens[1])){		// User in channel ?
						s_Channel* chan = _channels->getChannel(tokens[1]);
						sendMsgToList(fd, "PRIVMSG " + tokens[1] + " :" + tokens[2], chan->users);
					} else {
						sendServerMessage(fd, ERR_NOTONCHANNEL, "PRIVMSG :You are not in channel " + tokens[1]);
					}
				} else
					sendServerMessage(fd, ERR_NOSUCHCHANNEL, "PRIVMSG :No such channel " + tokens[1]);
			} else {
				userInfos* dest = _users->getUserByNick(tokens[1]);
				if (dest){
					sendMessage(dest->getFd(), ":" + _users->getUserByFd(fd)->getNickname() + " PRIVMSG " + tokens[1] + " :" + tokens[2]);
				} else err = ERR_NOSUCHNICK;
			}
		} else err = ERR_NOTEXTTOSEND;
	} else err = ERR_NORECIPIENT;

	switch (err){
		case ERR_NORECIPIENT:
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NORECIPIENT) + " " + _users->getUserByFd(fd)->getNickname() + " :No destinataire");
			break;
		case ERR_NOTEXTTOSEND:
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NOTEXTTOSEND) + " " + _users->getUserByFd(fd)->getNickname() + " :No text to send");
			break;
		case ERR_NOSUCHCHANNEL:
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NOSUCHCHANNEL) + " " + _users->getUserByFd(fd)->getNickname() + " " + tokens[1] + " :No such channel");
			break;
		case ERR_NOSUCHNICK:
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NOSUCHNICK) + " " + _users->getUserByFd(fd)->getNickname() + " " + tokens[1] + " :No such nickname");
			break;
	}
}

