#include "../includes/server.class.hpp"

void Server::cmd_whois(int fd, vector<string> tokens){
	userInfos* user = _users->getUserByFd(fd);
	if (tokens.size() < 2){		// No nickname to search
		_term.prtTmColor("WHOIS: No nickname to search", Terminal::RED);
		sendMessage(fd, ":" + _servername + " " + toString(ERR_NONICKNAMEGIVEN) + " " + user->getNickname() + " :Nickname missing");
		return;
	}
	userInfos* target = _users->getUserByNick(tokens[1]);
	if (!target){					// No target found
		_term.prtTmColor("WHOIS: Nickname not found", Terminal::RED);
		sendMessage(fd, ":" + _servername + " " + toString(ERR_NOSUCHNICK) + " " + user->getNickname() + " " + tokens[1]);
		return;
	}
	sendMessage(fd, ":" + _servername + " " + toString(RPL_WHOISUSER) + " " + user->getNickname() + " "
		+ target->getNickname() + " " + target->getUsername() + " " + _servername + " * :" + target->getRealname());

	sendMessage(fd, ":" + _servername + " " + toString(RPL_ENDOFWHOIS) + " " + user->getNickname() + " " + target->getNickname());
}
