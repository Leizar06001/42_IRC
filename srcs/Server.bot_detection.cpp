#include "../includes/server.class.hpp"

bool Server::isBotTraffic(const string& str){
	string banned_cmds[] = {"HTTP", "fox", "{", "app", "vm", "host", "os","GET","Host","Accept","User",
			"Referer","Accept-Encoding","Accept-Language","Cookie","Connection","Upgrade-Insecure-Requests",
			"Cache-Control","Pragma","DNT","Upgrade","If-Modified-Since","If-None-Match","If-Range",
			"If-Unmodified-Since","Range","TE","Trailer","Transfer-Encoding","Upgrade","Via","Warning",
			"X-", "OPTIONS","stats"};

	for(size_t i = 0; i < sizeof(banned_cmds) / sizeof(banned_cmds[0]); ++i){
		if (str.substr(0, banned_cmds[i].size()) == banned_cmds[i]) return true;
	}

	for(size_t i = 0; i < str.size(); ++i){
		if ((str[i] < 32) && str[i] != '\r' && str[i] != '\n') return true;
	}
	return false;
}
