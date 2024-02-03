#include "../includes/server.class.hpp"

//:server 302 YourNick :nickname=*+user@host


void Server::cmd_userhost(int fd, vector<string> tokens){
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;
	if (tokens.size() < 2){		// No nickname to search
		sendServerMessage(fd, ERR_NONICKNAMEGIVEN, ":Nickname missing");
		return;
	}
	userInfos* target = _users->getUserByNick(tokens[1]);
	if (!target){					// No target found
		sendServerMessage(fd, ERR_NOSUCHNICK, tokens[1]);
		return;
	}
	string status = target->getStatus() == "H" ? "+" : "-";
	//string ope = target->isOpe() ? "*" : "";
	string ope = "";
	if (user == target)	// SELF TARGETTING GIVE IP
		sendServerMessage(fd, RPL_USERHOST, ":" + target->getNickname() + "=" + ope + status + target->getUsername() + "@" + user->getIpAdress());
	else
		sendServerMessage(fd, RPL_USERHOST, ":" + target->getNickname() + "=" + ope + status + target->getUsername() + "@" + _servername);
	sendServerMessage(fd, RPL_ENDOFWHO, target->getNickname() + " :End of userhost list");
}
