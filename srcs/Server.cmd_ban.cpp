#include "../includes/server.class.hpp"

void Server::cmd_ban(int fd, vector<string> tokens){
	// Only for admin, for banning ip
	if (tokens.size() < 2){
		_term.prtTmColor("BAN: No ip\n", Terminal::RED);
		return;
	}
	userInfos* user = _users->getUserByFd(fd);
	if (!user || !user->isAdmin()){
		_term.prtTmColor("BAN: Not admin\n", Terminal::RED);
		return;
	}
	if (tokens[1].size() > 7 && tokens[1].size() < 16){
		addToBannedList(tokens[1]);
		_term.prtTmColor("BAN: " + tokens[1] + " added to banned list\n", Terminal::GREEN);
	} else {
		_term.prtTmColor("BAN: Invalid ip\n", Terminal::RED);
	}

}
