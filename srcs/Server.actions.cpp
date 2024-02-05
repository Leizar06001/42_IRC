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
		// sendMessage(fd, ":" + _servername + " 001 " + user->getNickname() + " :Welcome to the iRisChat network, "
		// 				+ user->getNickname() + "!" + user->getUsername() + "@" + user->getIpAdress());
		// message = ":" + _servername + " 002 " + user->getNickname() + " :Your host is " + _servername + ", running version 0.1";
		// sendMessage(fd, message);

		sendMessage(fd, ":" + _servername + " " + user->getNickname() + " 001 :Welcome to the iRisChat Network, " + user->getNickname());
		sendMessage(fd, ":" + _servername + " " + user->getNickname() + " 002 :Your host is " + _servername + ", running version 0.1");
		sendMessage(fd, ":" + _servername + " " + user->getNickname() + " 003 :This server was created at the beginning of the universe");
		sendMessage(fd, ":" + _servername + " " + user->getNickname() + " 004 " +_servername + " 0.1 iox ntb");
		sendMessage(fd, ":" + _servername + " " + user->getNickname() + " 005 ");

		_users->validateRegistration(user);
		vector<string> tokens;
		tokens.push_back("JOIN");
		tokens.push_back("#General");
		cmd_join(fd, tokens);
	}

	_users->rmFirstAction();
}
