#include "../includes/server.class.hpp"

static void toUpperCase(std::string& str) {
	for (size_t i = 0; i < str.size(); ++i) {
		str[i] = std::toupper(static_cast<unsigned char>(str[i]));
	}
}

void Server::analyseCommands(int fd, vector<string>& tokens){
	toUpperCase(tokens[0]);
	void (Server::*functionsPTRS[])(int fd,  vector<string> tokens) = {
		&Server::cmd_cap,
		&Server::cmd_pass,
		&Server::cmd_nick,
		&Server::cmd_user,
		&Server::cmd_ping,
		&Server::cmd_pong,
		&Server::cmd_msg,
		&Server::cmd_who,
		&Server::cmd_whois,
		&Server::cmd_names,
		&Server::cmd_quit,
		&Server::cmd_join,
		&Server::cmd_kick,
		&Server::cmd_invite,
		&Server::cmd_topic,
		&Server::cmd_mode,
		&Server::cmd_part,
		&Server::cmd_ban,
		&Server::cmd_userhost,
		&Server::cmd_notice,
		&Server::cmd_list
	};
	std::string cmds[] = {"CAP", "PASS", "NICK", "USER", "PING", "PONG", "PRIVMSG", "WHO", "WHOIS",
		"NAMES", "QUIT", "JOIN", "KICK", "INVITE", "TOPIC", "MODE", "PART", "BAN", "USERHOST", "NOTICE", "LIST"};

	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;
	size_t max_cmd_rights = user->isRegistered() ? sizeof(cmds) / sizeof(cmds[0]) : 4; // IF NOT REGISTERED, ALLOW ONLY FIRST 3 COMMANDS

	int found = 0;
	for (size_t i = 0; i < max_cmd_rights; ++i){
		if (cmds[i] == tokens[0]){
			(this->*functionsPTRS[i])(fd, tokens);
			found = 1;
			break;
		}
	}

	if (!found){		// CHECK IF THE COMMAND IS A IRC
		string all_cmds[] = {"CAP", "AUTHENTICATE", "PASS", "NICK", "USER", "PING", "PONG", "JOIN", "PART",
							"MODE", "TOPIC", "NAMES", "LIST", "INVITE", "KICK", "PRIVMSG", "NOTICE", "MOTD", "HELP",
							"LUSERS", "VERSION", "STATS", "LINKS", "TIME", "CONNECT", "TRACE", "ADMIN", "INFO",
							"SERVLIST", "SQUERY", "WHO", "WHOIS", "WHOWAS", "KILL", "PING", "PONG", "ERROR",
							"AWAY", "REHASH", "DIE", "RESTART", "SUMMON", "USERS", "WALLOPS", "USERHOST", "ISON"};
		for(size_t i = 0; i < sizeof(all_cmds) / sizeof(all_cmds[0]); ++i){
			if (all_cmds[i] == tokens[0]){
				found = 1;
				break;
			}
		}
	}
	if (!found){
		_term.prtTmColor("Command not found: " + tokens[0], Terminal::BRIGHT_RED);
		userInfos* user = _users->getUserByFd(fd);
		if (user){
			user->incWrongCmds();
			_term.prtTmColor(user->getNickname() + " has " + toString(user->getWrongCmdsNb()) + " wrong commands", Terminal::RED);
		}
		if (user->getWrongCmdsNb() > 10){
			_channels->quitServer(user);
			rmUser(fd, string("QUIT Too many wrong messages"));
		}
	}
}

vector<string> Server::parseMessage(int fd, string& msg){
	vector<string> 	tokens;
	string 			token;

	// Split the message at ':' if exists
	size_t	colonPos = msg.find(" :");
    string	beforeColon = (colonPos != string::npos) ? msg.substr(0, colonPos) : msg;
    string	afterColon = (colonPos != string::npos) ? msg.substr(colonPos + 2) : "";

	// Split at spaces
	stringstream 	ss(beforeColon);
	while (std::getline(ss, token, ' ')) {
		if (!token.empty()) {
			tokens.push_back(token);
		}
	}
	// Push back after ':' part
	if (!afterColon.empty()) tokens.push_back(afterColon);

	// Print message
	userInfos* user = _users->getUserByFd(fd);
	string name = user->getNickname();
	if (name.empty()) name = "FD." + toString(fd);
	//_term.prtTmColor("IN: " + name + ": " + msg, Terminal::BRIGHT_YELLOW);
	string str = "IN: " + Terminal::YELLOW + name + ": " + Terminal::BRIGHT_GREEN + tokens[0];
	for(size_t ind = 1; ind < tokens.size(); ++ind)
		str = str + Terminal::BRIGHT_BLUE + " [" + Terminal::BRIGHT_WHITE + tokens[ind] + Terminal::BRIGHT_BLUE + "]";
	_term.prtTmColor(str, Terminal::BRIGHT_YELLOW);

	user->resetLastMessageTime();
	return tokens;
}
