#include "../includes/server.class.hpp"

void Server::cmd_whois(int fd, vector<string> tokens){
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;
	if (tokens.size() < 2){		// No nickname to search
		_term.prtTmColor("WHOIS: No nickname to search", Terminal::RED);
		sendServerMessage(fd, ERR_NONICKNAMEGIVEN, user->getNickname() + " :Nickname missing");
		return;
	}
	userInfos* target = _users->getUserByNick(tokens[1]);
	if (!target){					// No target found
		_term.prtTmColor("WHOIS: Nickname not found", Terminal::RED);
		sendServerMessage(fd, ERR_NOSUCHNICK, user->getNickname() + " " + tokens[1]);
		return;
	}
	if (target == user || !target->isHideHost())
		sendServerMessage(fd, RPL_WHOISUSER, target->getNickname() + " " + target->getUsername() + " " + target->getIpAdress() + " * :" + target->getRealname());
	else
		sendServerMessage(fd, RPL_WHOISUSER, target->getNickname() + " " + target->getUsername() + " " + _servername + " * :" + target->getRealname());

	// Send channels list
	const map<string, s_Channel*> *chans = target->getChannels();
	map<string, s_Channel*>::const_iterator it = chans->begin();
	string chans_str = "";
	for(; it != chans->end(); ++it){
		chans_str += it->first + " ";
	}
	if (chans_str.length() > 0)
		sendServerMessage(fd, RPL_WHOISCHANNELS, target->getNickname() + " :" + chans_str);

	// END OF WHOIS
	sendServerMessage(fd, RPL_ENDOFWHOIS, target->getNickname() + ":END of WHOIS list");
}
