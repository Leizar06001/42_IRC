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
			vector<userInfos*> list = chan->users;
			for(vector<userInfos*>::iterator it = list.begin(); it != list.end(); ++it){
				sendServerMessage(fd, RPL_WHOREPLY, tokens[1] + " " + (*it)->getUsername() + " " + (*it)->getIpAdress() + " " + _servername + " "
					+ (*it)->getNickname() + " " + (*it)->getStatus() + " :0 " + (*it)->getRealname());
			}
			sendServerMessage(fd, RPL_ENDOFWHO, tokens[1] + " :End of /WHO list");
		}
	}
}

// send detailled infos list of users in channel or matching the pattern
// one line per user, followed by ENDOFWHO
// :server 352 YourNick #channel User Host Server Nick H :0 Real Name
