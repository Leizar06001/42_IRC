#include "../includes/server.class.hpp"

void Server::cmd_quit(int fd, vector<string> tokens){
	// Advertise other users
	// userInfos* user = _users->getUserByFd(fd);
	// if (!user) return ;
	// string nickname = user->getNickname();
	// string username = user->getUsername();
	string reason = (tokens.size() < 2) ? "" : tokens[1];

	// userInfos* target = _users->getNextUser(1);

	// while (target){
	// 	int fd_dest = target->getFd();
	// 	if (fd != fd_dest)
	// 		sendMessage(fd_dest, ":" + nickname + "!" + username + "@" + _servername + " QUIT :" + reason);

	// 	target = _users->getNextUser(0);
	// }

	sendMsgToList(fd, "QUIT :" + reason, _users->getIDmap());

	rmUser(fd, string("QUIT " + reason));
}
