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

		if (nick == tokens[1]) {		// SELF TARGETTING

			if (tokens.size() > 2){		// Try to set mode
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
			}
			// SEND MODES
			sendServerMessage(fd, RPL_UMODEIS, user->getUserMode());
		} else {	// ERR cant change mode for other users
			if (tokens.size() > 2){
				sendServerMessage(fd, ERR_USERSDONTMATCH, ":Cannot change mode for other users");
			} else {
				userInfos* target = _users->getUserByNick(tokens[1]);
				if (target){
					string target_mode = target->getUserMode();
					if (target_mode.length() > 0)
						sendMessage(fd, ":" + _servername + " MODE " + "#General" + " " + target_mode + " " + target->getNickname() + " " + target->getNickname());
				}

			}
			// sendServerMessage(fd, ERR_USERSDONTMATCH, ":Cannot change mode for other users");

		}
	}
}

//   :YourNick!YourIdent@YourHost MODE YourNick :+i
//	:irc.example.com 501 YourNick :Unknown mode flag
//	:irc.example.com 502 YourNick :Cannot change mode for other users
