#include "../includes/server.class.hpp"

void Server::cmd_ping(int fd, vector<string> tokens){
	if (tokens[1].empty())
		sendMessage(fd, string("PONG"));
	else
		sendMessage(fd, string("PONG :" + tokens[1]));
}
