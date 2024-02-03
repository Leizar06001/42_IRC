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
			sendMessage(fd, ":" + _servername + " " + toString(ERR_NOSUCHCHANNEL) + " " + nick + " " + tokens[1] + " :No such channel");
		} else {	// send list
			msg = ":" + _servername + " " + toString(RPL_NAMREPLY) + " " + nick + " = " + tokens[1] + " :" + _channels->getUsersNicksInChan(tokens[1]);
			sendMessage(fd, msg);
			msg = ":" + _servername + " " + toString(RPL_ENDOFNAMES) + " " + nick + " " + tokens[1] + " :End of /NAMES list";
			sendMessage(fd, msg);
		}
	} else {		// list all users
		msg = ":" + _servername + " " + toString(RPL_NAMREPLY) + " " + nick + " = * :" + _users->getListOfUsers();
		sendMessage(fd, msg);
		msg = ":" + _servername + " " + toString(RPL_ENDOFNAMES) + " " + nick + " * :End of /NAMES list";
		sendMessage(fd, msg);
	}
}

// should list all visible users
// should also add @ in front of operators
