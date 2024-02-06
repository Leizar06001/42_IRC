#include "../includes/server.class.hpp"

void Server::cmd_ping(int fd, vector<string> tokens){
	if (tokens.size() < 2)
		sendMessage(fd, "PONG " + _servername);
	else
		sendMessage(fd, "PONG " + _servername + " :" + tokens[1]);
}
