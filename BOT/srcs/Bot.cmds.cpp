#include "../includes/Bot.class.hpp"

void	Bot::cmd_ping(const vector<string>& tokens){
	if (tokens.size() > 1){
		sendMsg("PONG " + tokens[1]);
	}
}

void	Bot::cmd_privmsg(const vector<string>& tokens){
	string nick = getNickFromMsg(tokens[0]);
	map<string, s_user*>::iterator it = _nickMap.find(nick);

	if (it == _nickMap.end()){	// Create new user
		_nickMap.insert(make_pair<string, s_user*>(nick, createNewUser(nick)));
		prt(nick + " added to map\n", WHITE);
	} else {
		// Check times to reset warnings
		if (time(NULL) - it->second->last_warning_forbidden > TIME_BEFORE_KICK_FORBIDDEN){
			it->second->nb_warnings_forbidden = 0;
		}
	}

	if (tokens.size() > 3){
		string word = checkForbiddenWords(tokens[3]);

		if (word.length() > 0){

			if (it != _nickMap.end()){   // Check if user exceeded warnings max
				if (it->second->nb_warnings_forbidden >= WARNINGS_BEFORE_KICK_FORBIDDEN){
					sendMsg("KICK #General " + nick + " :Your recent messages contain inappropriate language. We encourage respectful communication in this community.");
				}
			}

			prt("Forbidden word: " + word + "\n", BRIGHT_RED);
			sendMsg("PRIVMSG #General :" + nick + " this kind of word is forbidden on this channel \"" + word + "\", you'll be kicked if you continue !");

			// Add one warning
			it = _nickMap.find(nick);
			++(it->second->nb_warnings_forbidden);
			it->second->last_warning_forbidden = time(NULL);
			prt(nick + " nb warnings: " + toString(it->second->nb_warnings_forbidden) + "\n", WHITE);
		}
	}
}


void	Bot::chooseCmd(const vector<string>& tokens){
	string cmds_first_pos[] = {"PING"};

	void (Bot::*functionsPTRSfirst[])(const vector<string>& tokens) = {
		&Bot::cmd_ping
	};

	for (size_t i = 0; i < sizeof(cmds_first_pos) / sizeof(cmds_first_pos[0]); ++i){
		if (cmds_first_pos[i] == tokens[0]){
			(this->*functionsPTRSfirst[i])(tokens);
			break;
		}
	}

	string cmds_sec_pos[] = {"PRIVMSG"};

	void (Bot::*functionsPTRSsec[])(const vector<string>& tokens) = {
		&Bot::cmd_privmsg
	};

	for (size_t i = 0; i < sizeof(cmds_sec_pos) / sizeof(cmds_sec_pos[0]); ++i){
		if (cmds_sec_pos[i] == tokens[1]){
			(this->*functionsPTRSsec[i])(tokens);
			break;
		}
	}
}

const string	Bot::getNickFromMsg(const string& str){
	string nick = str.substr(1, str.find("!", 1) - 1);
	return nick;
}
