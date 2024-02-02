#include "../includes/server.class.hpp"

void Server::cmd_join(int fd, vector<string> tokens){
	if (tokens.size() < 2){
		// NO CHANNEL NAME
		return;
	}
	// JOIN CHANNEL
	userInfos* user = _users->getUserByFd(fd);
	string chanType = "#";
	sendMessage(fd, ":" + user->getNickname() + "!" + user->getUsername() + "@" + _servername + " JOIN " + chanType + tokens[1]);

	// NOTIF OTHER USERS IN CHANNEL
	int nb_users = _users->getNbUsers();
	userInfos* target = _users->getNextUser(1);
	int i = 0;
	while (i < nb_users){
		if (target){
			int fd_dest = target->getFd();
			if (fd != fd_dest)
				sendMessage(fd_dest, ":" + user->getNickname() + "!" + user->getUsername() + "@" + _servername + " JOIN " + chanType + tokens[1]);
			++i;
			target = _users->getNextUser(0);
		}
	}
}