#include "../includes/server.class.hpp"
#include "../includes/sha256.hpp"

void	Server::cmd_pass(int fd, vector<string> tokens){
	if (tokens.size() < 2){
		sendMessage(fd, ":" + _servername + " " + toString(ERR_NEEDMOREPARAMS) + " :Password is empty");
		rmUser(fd, string("Empty password"));
		return ;
	}
	if (tokens[1] == _password){
		userInfos* user = _users->getUserByFd(fd);
		if (!user) return;
		user->setPasswordOk();
	} else {
			// CHECK FOR ADMIN PASSWORD, SHA 256 in config file
		string pass_sha = sha256(tokens[1]);
		if (pass_sha == _admin_password){
			_term.prtTmColor("ADMIN CONNECTED", Terminal::GREEN);
			userInfos* user = _users->getUserByFd(fd);
			if (!user) return;
			user->setPasswordOk();
			user->setAdmin();
			return ;
		}

		// WRONG PASSWORD
		sendMessage(fd, ":" + _servername + " " + toString(ERR_PASSWDMISMATCH) + " :Wrong password");
		rmUser(fd, string("Wrong password"));
	}
}
