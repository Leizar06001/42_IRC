#include "../includes/server.class.hpp"

void Server::checkClientRegistrationTimeout(void){
	int fdtm = _users->checkRegistrationTimeout(REGISTRATION_TIMEOUT);
	if (fdtm >= 0){
		rmUser(fdtm, string("REGISTRATION TIMEOUT"));
	}
}

void Server::checkIfClientsAlive(void){
	if (time(NULL) - _last_timeout_check > TIMEOUT_CHECK_TIME){
		// _term.prtTmColor("Checking timeouts..", Terminal::BRIGHT_CYAN);

		vector<userInfos*> list = _users->checkConnectionTimeout(CONNECTION_TIMEOUT);
		vector<userInfos*>::iterator it = list.begin();

		while (it != list.end()){
			// _term.prtTmColor((*it)->getNickname() + " Ping? " + toString((*it)->getPong()) + " t: " + toString(time(NULL) - (*it)->getLastMessageTime()), Terminal::BRIGHT_CYAN);
			if (!(*it)->getPong()){	// send PING to check alive
				(*it)->setPong(true);
				sendMessage((*it)->getFd(), "PING :test_alive");
			} else {
				vector<string> tokens;
				tokens.push_back("QUIT");
				tokens.push_back("Connection timeout");
				cmd_quit((*it)->getFd(), tokens);
			}
			++it;
		}
		_last_timeout_check = time(NULL);
	}
}
