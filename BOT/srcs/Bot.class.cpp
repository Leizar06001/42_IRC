#include "../includes/Bot.class.hpp"
#include "../includes/prt.hpp"
#include "../includes/swears.hpp"

Bot::Bot(string nickname, string realname):_connected(0), _botnick(nickname), _botname(nickname), _botreal(realname), _data_buffer(""){
	prt("BOT Created: " + nickname + "\n", BRIGHT_YELLOW);
	_forbiddenWords = getForbiddenWords();
	_socket = -1;
}
Bot::~Bot(void){
	close(_socket);
	map<string, s_user*>::iterator it = _nickMap.begin();
	for(; it != _nickMap.end(); ++it){
		delete it->second;
	}
	prt("BOT DESTROYED\n", BRIGHT_YELLOW);
}

int		Bot::botLoop(void){
	if (_connected == false) return 1;

	readSocket();

	vector<string> tokens;
	if ((tokens = parseNext()).size() > 0){
		chooseCmd(tokens);
	}

	// Check banned users
	map<string, time_t>::iterator it = _bannedUsers.begin();
	while (it != _bannedUsers.end()){
		if (time(NULL) - it->second > TIME_BEFORE_UNBAN){
			prt(it->first + " unbanned\n", BRIGHT_GREEN);
			sendMsg("MODE #General -b " + it->first);
			_bannedUsers.erase(it++);
		} else {
			++it;
		}
	}

	return 0;
}

void	Bot::sendMsg(const string &msg){
	string out = msg + "\r\n";
	send(_socket, out.c_str(), out.length(), MSG_NOSIGNAL);
	prt("<< " + msg + "\n", YELLOW);
}

s_user* Bot::createNewUser(const string& nick){
	s_user *user = new s_user;
	user->nick = nick;
	user->nb_warnings_forbidden = 0;
	user->last_warning_forbidden = 0;
	user->nb_warnings_spam = 0;
	user->nb_times_kicked = 0;
	user->last_kick = 0;
	user->banned = false;
	prt(nick + " new user create\n", BRIGHT_CYAN);
	return user;
}

