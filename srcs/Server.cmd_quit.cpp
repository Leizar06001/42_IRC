#include "../includes/server.class.hpp"

void Server::cmd_quit(int fd, vector<string> tokens){
	// Advertise other users
	userInfos* user = _users->getUserByFd(fd);
	string nickname = user->getNickname();
	string username = user->getUsername();
	string reason;
	(tokens.size() < 2) ? reason = "" : reason = tokens[1];

	int nb_users = _users->getNbUsers();
	userInfos* target = _users->getNextUser(1);
	int i = 0;
	if (nb_users > 1){
		while (i < nb_users){
			if (target){
				int fd_dest = target->getFd();
				if (fd != fd_dest)
					sendMessage(fd_dest, ":" + nickname + "!" + username + "@" + _servername + " QUIT :" + reason);
				++i;
				target = _users->getNextUser(0);
			}
		}
	}
	rmUser(fd, string("QUIT " + reason));
}
