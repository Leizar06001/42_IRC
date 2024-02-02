#include "../includes/server.class.hpp"

void Server::checkPendingActions(void){
	userInfos* user;
	while ((user = _users->getUserActionRequests()) != NULL){
		performAction(user);
	}
}

void Server::performAction(userInfos* user){
	int action_type = user->getAction();
	string msg_act;
	switch (action_type){
		case ACT_REGISTRATION:
			msg_act = "REGISTRATION"; break;
	}
	_term.prtTmColor("User " + user->getNickname() + " request action " + msg_act + "\n", Terminal::BRIGHT_CYAN);

	int fd = user->getFd();

	if (action_type == ACT_REGISTRATION){
		string message = ":" + _servername + " 001 " + user->getNickname() + " :Welcome to the iRisChat network, " + user->getNickname() + "!" + user->getUsername() + "@" + _servername;
		sendMessage(fd, message);
		// message = ":" + _servername + " 002 " + user->getNickname() + " :Your host is " + _servername + ", running version 0.1";
		// sendMessage(fd, message);
		_users->validateRegistration(user);
		vector<string> tokens;
		tokens.push_back("*");
		tokens.push_back("General");
		cmd_join(fd, tokens);
		cmd_names(fd, tokens);
	}

	_users->rmFirstAction();
}