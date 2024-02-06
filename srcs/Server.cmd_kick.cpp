#include "../includes/server.class.hpp"

void Server::cmd_kick(int fd, vector<string> tokens){
	if (tokens.size() < 3){
		// NO KICKNAME
		return;
	}
	userInfos* moder = _users->getUserByFd(fd);
	if (!moder) return;
	userInfos* user = _users->getUserByNick(tokens[2]);
	if (!user){
		// NO SUCH NICK
		return ;
	}

	int ret = _channels->kickChannel(user, tokens[1]);
	if (ret == 0){
		_term.prtTmColor(tokens[2] + " kicked of channel " + tokens[1], Terminal::BRIGHT_RED);
		sendMsgToList(fd, "KICK " + tokens[1] + " " + tokens[2] + " :" + moder->getNickname(), _channels->getChannel(tokens[2])->users);
	}
}
