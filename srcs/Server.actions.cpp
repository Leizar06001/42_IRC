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

		writeToConLog(timestamp_fail2ban() + " CLIENT: " + _users->getUserByFd(fd)->getIpAdress() + " Registered: " + user->getNickname());

		sendMessage(fd, ":" + _servername + " 001 " + user->getNickname() + " :Welcome to the iRisChat Network, " + user->getNickname());
		sendMessage(fd, ":" + _servername + " 002 " + user->getNickname() + " :Your host is " + _servername + ", running version " + _version);
		sendMessage(fd, ":" + _servername + " 003 " + user->getNickname() + " :This server was created the " + str_time);
		sendMessage(fd, ":" + _servername + " 004 " + user->getNickname() + " " +_servername + " 0.1 iox ntilm");
		sendMessage(fd, ":" + _servername + " 005 " + user->getNickname() + " TARGMAX=NAMES:1,LIST:1,KICK:1,WHOIS:1,PRIVMSG:1 CHANLIMIT=#:" + toString(_max_channels) + " NICKLEN=16 MAXNICKLEN=16 CHANNELLEN=16");
		sendServerMessage(fd, RPL_LUSERME, " :I have " + toString(_users->getNbUsers()) + " users and 1 servers");
		sendServerMessage(fd, RPL_LUSERCHANNELS, toString(_channels->getNbChannel()) + " :channels formed");
		sendServerMessage(fd, RPL_MOTD, " :- " + _servername + " Message of the day - ");
		sendServerMessage(fd, RPL_MOTD, " :- Welcome to the iRisChat Network");
		sendServerMessage(fd, RPL_MOTD, " :- Enjoy your stay and have fun!");
		sendServerMessage(fd, RPL_ENDOFMOTD, " :End of /MOTD command.");


		_users->validateRegistration(user);
		vector<string> tokens;
		tokens.push_back("JOIN");
		tokens.push_back("#General");
		// cmd_list(fd, tokens);
		cmd_join(fd, tokens);

	}

	_users->rmFirstAction();
}
