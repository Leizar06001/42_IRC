#include "../includes/server.class.hpp"

bool Server::isBotTraffic(const string& str){
	if (str.substr(0, 4) == "HTTP") return true;

	return false;
}
