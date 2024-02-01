#include "../includes/server.class.hpp"

void Server::analyseCommands(int fd, vector<string>& tokens){
	void (Server::*functionsPTRS[])(int fd,  vector<string> tokens) = {
		&Server::cmd_cap,
		&Server::cmd_nick,
		&Server::cmd_user,
		&Server::cmd_ping,
		&Server::cmd_pong,
		&Server::cmd_msg,
		&Server::cmd_whois,
		&Server::cmd_names,
		&Server::cmd_quit,
		&Server::cmd_join
	};
	std::string cmds[] = {"CAP", "NICK", "USER", "PING", "PONG", "PRIVMSG", "WHOIS", "NAMES", "QUIT", "JOIN"};

	for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); ++i){
		if (cmds[i] == tokens[0]){
			(this->*functionsPTRS[i])(fd, tokens);
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
