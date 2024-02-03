#include "../includes/server.class.hpp"

void Server::cmd_mode(int fd, vector<string> tokens){
	if (tokens.size() < 2) return;
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;

	string	nick = user->getNickname();

	if (tokens[1][0] == '#'){		// target is a channel
		s_Channel* chan = _channels->getChannel(tokens[1]);
		if (chan == NULL){ // channel not exists
			sendServerMessage(fd, ERR_NOSUCHCHANNEL, nick + " :" + tokens[1]);
		} else {
			sendServerMessage(fd, RPL_CHANNELMODEIS, nick + " " + tokens[1] + " " + chan->mode);
		}




	} else {						// target is a user
		userInfos* target = _users->getUserByNick(tokens[1]);
		if (!target){	// NO SUCH USER
			sendServerMessage(fd, ERR_NOSUCHNICK, nick + " :" + tokens[1]);
			return;
		}
		if (tokens.size() < 3){		// Getting mode infos
			sendClientMessage(fd, "MODE " + nick + " " + target->getUserMode());
		} else {					// Try to set mode
			if (nick == tokens[1]) {		// SELF TARGETTING
				user->setUserMode(tokens[2]);
				sendClientMessage(fd, "MODE " + nick + " " + user->getUserMode());
			} else {	// ERR cant change mode for other users
				sendServerMessage(fd, ERR_USERSDONTMATCH, nick + ":Cannot change mode for other users");
			}
		}

	}
}

//   :YourNick!YourIdent@YourHost MODE YourNick :+i
//	:irc.example.com 501 YourNick :Unknown mode flag
//	:irc.example.com 502 YourNick :Cannot change mode for other users
