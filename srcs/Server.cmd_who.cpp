#include "../includes/server.class.hpp"

void Server::cmd_who(int fd, vector<string> tokens){
	userInfos* 	user = _users->getUserByFd(fd);
	if (!user) return ;
	string		nick = user->getNickname();
	if (tokens.size() < 2){
		sendServerMessage(fd, ERR_NEEDMOREPARAMS, "");
		return;
	}
	string		msg;
	if (nick.length() == 0) return ;

	if (tokens[1][0] == '#'){	// list users of a channel
		s_Channel* chan = _channels->getChannel(tokens[1]);
		if (!chan){	// channel not found
			sendServerMessage(fd, ERR_NOSUCHCHANNEL, tokens[1] + " :No such channel");
		} else {	// send list
			map<string, userInfos*> list = chan->users;
			for(map<string, userInfos*>::iterator it = list.begin(); it != list.end(); ++it){
				if (it->second == user){		// SELF TARGETTING GIVE IP
					sendServerMessage(fd, RPL_WHOREPLY, tokens[1] + " " + it->second->getUsername() + " " + it->second->getIpAdress() + " " + _servername
						+ " " + _channels->getUserPriviledges(it->second->getNickname(), tokens[1]) + it->second->getNickname() + " H :0 " + it->second->getRealname());
				} else {
					sendServerMessage(fd, RPL_WHOREPLY, tokens[1] + " " + it->second->getUsername() + " " + _servername + " " + _servername + " "
						+ _channels->getUserPriviledges(it->second->getNickname(), tokens[1]) + it->second->getNickname() + " H :0 " + it->second->getRealname());
				}
			}
			sendServerMessage(fd, RPL_ENDOFWHO, tokens[1] + " :End of /WHO list");
		}


	} else {					// Get one user details
		userInfos* target = _users->getUserByNick(tokens[1]);
		if (!target){
			sendServerMessage(fd, ERR_NOSUCHNICK, tokens[1] + " :No such nickname");
		} else {
			if (target == user){		// SELF TARGETTING GIVE IP
				sendServerMessage(fd, RPL_WHOREPLY, "#General " + target->getUsername() + " " + target->getIpAdress() + " " + _servername + " "
						+ _channels->getUserPriviledges(target->getNickname(), "#General") + target->getNickname() + " H :0 " + target->getRealname());
			} else {
				sendServerMessage(fd, RPL_WHOREPLY, "#General " + target->getUsername() + " " + _servername + " " + _servername + " "
						+ _channels->getUserPriviledges(target->getNickname(), "#General") + target->getNickname() + " H :0 " + target->getRealname());
			}
			sendServerMessage(fd, RPL_ENDOFWHO, tokens[1] + " :End of /WHO list");
		}
	}
}

// send detailled infos list of users in channel or matching the pattern
// one line per user, followed by ENDOFWHO
// :server 352 YourNick #channel User Host Server Nick H :0 Real Name
