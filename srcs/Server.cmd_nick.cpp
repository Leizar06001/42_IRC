#include "../includes/server.class.hpp"

void Server::cmd_nick(int fd, vector<string> tokens){
	if (tokens.size() < 2){	// ERR not nickname
		sendMessage(fd, string(":" + _servername + " " + toString(ERR_NEEDMOREPARAMS) + " :No nickname given"));
		return;
	}
	userInfos* user = _users->getUserByFd(fd);
	if (tokens[1] == user->getNickname())
		return ;
	int ret = _users->setNickname(fd, tokens[1]);
	string nick = user->getNickname();
	if (nick.empty()) nick = tokens[1];
	if (ret == ERR_NICKNAMEINUSE){
		_term.prtTmColor("FD. " + toString(fd) + " nickname already in use\n", Terminal::RED);
		sendMessage(fd, string(":" + _servername + " " + toString(ERR_NICKNAMEINUSE) + " " + nick + " " + tokens[1] + " :Nickname already in use"));
		return ;
	} else if (ret == ERR_ERRONEUSNICKNAME){
		_term.prtTmColor("FD. " + toString(fd) + " wrong characters in nickname\n", Terminal::RED);
		sendMessage(fd, string(":" + _servername + " " + toString(ERR_ERRONEUSNICKNAME) + " " + nick + " " + tokens[1] + " :Erroneus nickname"));
		return ;
	} else if (ret == 1) { // Nick changed, must advertise new nick to others
		int nb_users = _users->getNbUsers();
		userInfos* target = _users->getNextUser(1);
		int i = 0;
		while (i < nb_users){
			if (target){
				int fd_dest = target->getFd();
				if (fd != fd_dest)
					sendMessage(fd_dest, ":" + user->getPrevNick() + "!" + user->getUsername() + "@" + _servername + " NICK " + nick);
				++i;
				target = _users->getNextUser(0);
			}
		}
	}
	sendMessage(fd, ":" + user->getPrevNick() + " NICK " + nick);
	_users->checkForRegistration(fd);
}
