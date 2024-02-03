#include "../includes/server.class.hpp"

void Server::cmd_mode(int fd, vector<string> tokens){
	if (tokens.size() < 2) return;
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;

	string	nick = user->getNickname();

	if (tokens[1][0] == '#'){		// target is a channel
		s_Channel* chan = _channels->getChannel(tokens[1]);
		if (chan == NULL){ // channel not exists
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NOSUCHCHANNEL) + " " + nick + " :" + tokens[1]);
		} else {
			sendMessage(fd, ":" + _servername + " " + toString(RPL_CHANNELMODEIS) + " " + nick + " " + tokens[1] + " " + chan->mode);
		}




	} else {						// target is a user
		userInfos* target = _users->getUserByNick(tokens[1]);
		if (!target){	// NO SUCH USER
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NOSUCHNICK) + " " + nick + " :" + tokens[1]);
			return;
		}
		if (tokens.size() < 3){		// Getting mode infos
			sendMessage(fd, ":" + nick + "!" + user->getUsername() + "@" + _servername + " MODE " + nick + " " + target->getUserMode());
		} else {					// Try to set mode
			if (nick == tokens[1]) {		// SELF TARGETTING
				user->setUserMode(tokens[2]);
				sendMessage(fd, ":" + nick + "!" + user->getUsername() + "@" + _servername + " MODE " + nick + " " + user->getUserMode());
			} else {	// ERR cant change mode for other users
				sendMessage(fd, ":" + _servername + " " + toString(ERR_USERSDONTMATCH) + " " + nick + ":Cannot change mode for other users");
			}
		}

	}
}

//   :YourNick!YourIdent@YourHost MODE YourNick :+i
//	:irc.example.com 501 YourNick :Unknown mode flag
//	:irc.example.com 502 YourNick :Cannot change mode for other users
