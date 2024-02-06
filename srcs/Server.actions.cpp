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

		tm* 	crea_time = localtime(&_creation_time);
		char 	buf[22];
		std::strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", crea_time);
		std::string		str_time = toString(buf);

		sendMessage(fd, ":" + _servername + " 001 " + user->getNickname() + " :Welcome to the iRisChat Network, " + user->getNickname());
		sendMessage(fd, ":" + _servername + " 002 " + user->getNickname() + " :Your host is " + _servername + ", running version 0.1");
		sendMessage(fd, ":" + _servername + " 003 " + user->getNickname() + " :This server was created the " + str_time);
		sendMessage(fd, ":" + _servername + " 004 " + user->getNickname() + " " +_servername + " 0.1 iox ntb");
		sendMessage(fd, ":" + _servername + " 005 " + user->getNickname() + " ");

		_users->validateRegistration(user);
		vector<string> tokens;
		tokens.push_back("JOIN");
		tokens.push_back("#General");
		// cmd_list(fd, tokens);
		cmd_join(fd, tokens);

	}

	_users->rmFirstAction();
}
