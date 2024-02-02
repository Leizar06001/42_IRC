#include "../includes/server.class.hpp"

void Server::cmd_cap(int fd, vector<string> tokens){
	if (!tokens.size() > 1){
		if (tokens[1] == "LS"){
			sendMessage(fd, string("CAP * LS :"));
		} else if (tokens[1] == "LIST"){
			sendMessage(fd, string("CAP * LIST :"));
		}
	}
}
