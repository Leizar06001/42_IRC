#include "../includes/server.class.hpp"

void Server::cmd_unban(int fd, vector<string> tokens){
	// Only for admin, for banning ip
	if (tokens.size() < 2){
		_term.prtTmColor("UNBAN: No ip\n", Terminal::RED);
		return;
	}
	userInfos* user = _users->getUserByFd(fd);
	if (!user || !user->isAdmin()){
		_term.prtTmColor("UNBAN: Not admin\n", Terminal::RED);
		return;
	}
	if (tokens[1].size() > 7 && tokens[1].size() < 16){
		_bans_ip.erase(std::remove(_bans_ip.begin(), _bans_ip.end(), tokens[1]), _bans_ip.end());
		_term.prtTmColor("UNBAN: " + tokens[1] + " removed from banned list\n", Terminal::GREEN);
	} else {
		_term.prtTmColor("UNBAN: Invalid ip\n", Terminal::RED);
	}

}
