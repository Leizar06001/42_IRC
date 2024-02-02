#include "../includes/server.class.hpp"

void Server::cmd_ping(int fd, vector<string> tokens){
	if (tokens.size() < 2)
		sendMessage(fd, string("PONG"));
	else
		sendMessage(fd, string("PONG :" + tokens[1]));
}
