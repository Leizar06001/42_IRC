#include "../includes/server.class.hpp"

void Server::cmd_msg(int fd, vector<string> tokens){
	int err = 0;
	if (tokens.size() > 1){
		if (tokens.size() > 2){
			// Check if dest is a channel
			if (tokens[1][0] == '#'){
				if (tokens[1] == "#General"){
					sendMsgToList(fd, "PRIVMSG " + tokens[1] + " :" + tokens[2], _users->getIDmap());
				} else err = ERR_NOSUCHCHANNEL;
			} else {
				userInfos* dest = _users->getUserByNick(tokens[1]);
				if (dest){
					sendMessage(dest->getFd(), ":" + _users->getUserByFd(fd)->getNickname() + " PRIVMSG " + tokens[1] + " :" + tokens[2]);
				} else err = ERR_NOSUCHNICK;
			}
		} else err = ERR_NOTEXTTOSEND;
	} else err = ERR_NORECIPIENT;

	switch (err){
		case ERR_NORECIPIENT:
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NORECIPIENT) + " " + _users->getUserByFd(fd)->getNickname() + " :No destinataire");
			break;
		case ERR_NOTEXTTOSEND:
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NOTEXTTOSEND) + " " + _users->getUserByFd(fd)->getNickname() + " :No text to send");
			break;
		case ERR_NOSUCHCHANNEL:
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NOSUCHCHANNEL) + " " + _users->getUserByFd(fd)->getNickname() + " " + tokens[1] + " :No such channel");
			break;
		case ERR_NOSUCHNICK:
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NOSUCHNICK) + " " + _users->getUserByFd(fd)->getNickname() + " " + tokens[1] + " :No such nickname");
			break;
	}
}

void	Server::sendMsgToList(int fd_source, const string& msg, const map<int, size_t> &lst){
	userInfos* source = _users->getUserByFd(fd_source);
	map<int, size_t>::const_iterator it = lst.begin();
	if (msg.length() == 0 || !source) return;
	while (it != lst.end()){
		if (it->first != fd_source){
			sendMessage(it->first, ":" + source->getNickname() + "!" + source->getUsername() + "@" + _servername + " " + msg);
		}
		++it;
	}
}

void	Server::sendMsgToList(int fd_source, const string& msg, const vector<userInfos*> &lst){
	userInfos* source = _users->getUserByFd(fd_source);
	vector<userInfos*>::const_iterator it = lst.begin();
	if (msg.length() == 0 || !source) return;
	while (it != lst.end()){
		if ((*it)->getFd() != fd_source){
			sendMessage((*it)->getFd(), ":" + source->getNickname() + "!" + source->getUsername() + "@" + _servername + " " + msg);
		}
		++it;
	}
}
