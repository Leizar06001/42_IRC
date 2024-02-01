#include "../includes/server.class.hpp"

void Server::cmd_pong(int fd, vector<string> tokens){
	(void)tokens;
	_users->getUserByFd(fd)->setPong(false);
}
