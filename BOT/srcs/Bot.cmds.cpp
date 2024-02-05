#include "../includes/Bot.class.hpp"



void	Bot::chooseCmd(const vector<string>& tokens){
	string cmds[] = {"PING"};

	void (Bot::*functionsPTRS[])(const vector<string>& tokens) = {
		&Bot::cmd_ping
	};

	for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); ++i){
		if (cmds[i] == tokens[0]){
			(this->*functionsPTRS[i])(tokens);
			break;
		}
	}
}

void	Bot::cmd_ping(const vector<string>& tokens){
	if (tokens.size() > 1){
		sendMsg("PONG " + tokens[1]);
	}
}
