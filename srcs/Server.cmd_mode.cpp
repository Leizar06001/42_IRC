#include "../includes/server.class.hpp"

void Server::cmd_mode(int fd, vector<string> tokens){
	if (tokens.size() < 2) return;
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;

	string	nick = user->getNickname();

	if (tokens[1][0] == '#'){		// target is a channel
		s_Channel* chan = _channels->getChannel(tokens[1]);
		if (chan == NULL){ // channel not exists
			sendServerMessage(fd, ERR_NOSUCHCHANNEL, tokens[1]);
		} else {
			sendServerMessage(fd, RPL_CHANNELMODEIS, tokens[1] + " " + chan->mode);
		}


	} else {						// target is a user
		userInfos* target = _users->getUserByNick(tokens[1]);
		if (!target){	// NO SUCH USER
			sendServerMessage(fd, ERR_NOSUCHNICK, tokens[1]);
			return;
		}
		if (tokens.size() < 3){		// NO MODE SPECIFIED : ONLY Getting mode infos
			sendMessage(fd, ":" + _servername + " " + toString(RPL_UMODEIS) + " " + target->getNickname() + " " + target->getUserMode());


		} else {					// Try to set mode
			if (nick == tokens[1]) {		// SELF TARGETTING
				bool add = 0;
				bool rm = 0;
				for(size_t i = 0; i < tokens[2].length(); ++i){
					switch (tokens[2][i]){
						case '+': add = 1; rm = 0; break;
						case '-': add = 0; rm = 1; break;
					}
					if (add && !rm){			// ADD mode
						switch (tokens[2][i]){
							case 'i': target->setInvisible(true); break;
							case 'x': break;	// hide host
						}
					} else if (!add && rm){		// RM mode
						switch (tokens[2][i]){
							case 'i': target->setInvisible(false); break;
							case 'x': break;	// hide host
						}
					}
				}
				sendServerMessage(fd, RPL_UMODEIS, user->getUserMode());
			} else {	// ERR cant change mode for other users
				sendServerMessage(fd, ERR_USERSDONTMATCH, ":Cannot change mode for other users");
			}
		}

	}
}

//   :YourNick!YourIdent@YourHost MODE YourNick :+i
//	:irc.example.com 501 YourNick :Unknown mode flag
//	:irc.example.com 502 YourNick :Cannot change mode for other users
