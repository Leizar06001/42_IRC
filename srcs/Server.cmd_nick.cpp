#include "../includes/server.class.hpp"

void Server::cmd_nick(int fd, vector<string> tokens){
	if (tokens.size() < 2){	// ERR not nickname
		sendMessage(fd, ":" + _servername + " " + toString(ERR_NEEDMOREPARAMS) + " :No nickname given");
		return;
	}
	userInfos* user = _users->getUserByFd(fd);
	if (tokens[1] == user->getNickname())
		return ;

	string prevnick = user->getNickname();
	int ret = _users->setNickname(fd, tokens[1]);
	string nick = user->getNickname();
	if (nick.empty()) nick = tokens[1];


	if (ret == ERR_NICKNAMEINUSE){
		_term.prtTmColor("FD. " + toString(fd) + " nickname already in use\n", Terminal::BRIGHT_RED);
		if (user->isRegistered())
			sendServerMessage(fd, ERR_NICKNAMEINUSE, tokens[1] + " :Nickname already in use");
		else
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NICKNAMEINUSE) + " * " + tokens[1]);
		return ;


	} else if (ret == ERR_ERRONEUSNICKNAME){
		_term.prtTmColor("FD. " + toString(fd) + " wrong characters in nickname\n", Terminal::BRIGHT_RED);
		sendServerMessage(fd, ERR_ERRONEUSNICKNAME, tokens[1] + " :Erroneus nickname");
		return ;


	} else if (ret == 1) { // Nick changed, must advertise new nick to others
		int nb_users = _users->getNbUsers();
		userInfos* target = _users->getNextUser(1);

		// first, change nickname in channels lists
		_channels->changeNickInChannels(prevnick, nick);

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
		sendMessage(fd, ":" + user->getPrevNick() + " NICK " + nick);
	}
	sendMessage(fd, ":NICK " + nick);
	_users->checkForRegistration(fd);
}
