#include "../includes/server.class.hpp"

void Server::cmd_names(int fd, vector<string> tokens){
	string	msg;
	string	nick = _users->getUserByFd(fd)->getNickname();

	// channels   = : public, * : secret, @ : private
	// users      @ : operator
	if (tokens.size() > 1){	// list users of a channel
		msg = ":" + _servername + " " + toString(RPL_NAMREPLY) + " " + nick + " = " + tokens[1] + " :" + _users->getListOfUsers();
		sendMessage(fd, msg);
		msg = ":" + _servername + " " + toString(RPL_ENDOFNAMES) + " " + nick + " " + tokens[1] + " :End of /NAMES list";
		sendMessage(fd, msg);
	} else {		// list all users
		msg = ":" + _servername + " " + toString(RPL_NAMREPLY) + " " + nick + " = * :" + _users->getListOfUsers();
		sendMessage(fd, msg);
		msg = ":" + _servername + " " + toString(RPL_ENDOFNAMES) + " " + nick + " * :End of /NAMES list";
		sendMessage(fd, msg);
	}
}
