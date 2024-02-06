#include "../includes/server.class.hpp"

void Server::cmd_quit(int fd, vector<string> tokens){
	string reason = (tokens.size() < 2) ? "" : tokens[1];

	sendMsgToList(fd, "QUIT :" + reason, _users->getIDmap());

	_channels->quitServer(_users->getUserByFd(fd));
	rmUser(fd, string("QUIT " + reason));
}
