#include "../includes/server.class.hpp"

void Server::cmd_names(int fd, vector<string> tokens){
	string		msg;
	userInfos* 	user = _users->getUserByFd(fd);
	if (!user) return ;
	string		nick = user->getNickname();
	if (nick.length() == 0) return ;
	// channels   = : public, * : secret, @ : private
	// users      @ : operator
	if (tokens.size() > 1){	// list users of a channel
		s_Channel* chan = _channels->getChannel(tokens[1]);
		if (!chan){	// channel not found
			sendServerMessage(fd, ERR_NOSUCHCHANNEL, tokens[1] + " :No such channel");
		} else {	// send list
			sendServerMessage(fd, RPL_NAMREPLY, tokens[1] + " :" + _channels->getUsersNicksInChan(tokens[1]));
			sendServerMessage(fd, RPL_ENDOFNAMES, tokens[1] + " :End of /NAMES list");
		}
	} else {		// list all users
		sendServerMessage(fd, RPL_NAMREPLY,"= * :" + _users->getListOfUsers());
		sendServerMessage(fd, RPL_ENDOFNAMES, "* :End of /NAMES list");
	}
}

// should list all visible users
// should also add @ in front of operators
