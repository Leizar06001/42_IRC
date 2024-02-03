#include "../includes/server.class.hpp"

void Server::cmd_join(int fd, vector<string> tokens){
	if (tokens.size() < 2){
		// NO CHANNEL NAME
		return;
	}
	// JOIN CHANNEL
	userInfos* user = _users->getUserByFd(fd);
	int ret = _channels->joinChannel(user, tokens[1]);

	if (ret != 0) return ;
	sendMessage(fd, ":" + user->getNickname() + "!" + user->getUsername() + "@" + _servername + " JOIN " + tokens[1]);

	// NOTIF OTHER USERS IN CHANNEL
	s_Channel* chan = _channels->getChannel(tokens[1]);
	if (!chan) return;
	sendMsgToList(fd, "JOIN " + tokens[1], chan->users);
	// SEND ALL USERS OF THE CHANNEL TO THE NEW
	vector<string> toks;
	toks.push_back("NAMES");
	toks.push_back(tokens[1]);
	toks.push_back(_channels->getUsersNicksInChan(tokens[1]));
	cmd_names(fd, toks);

}
