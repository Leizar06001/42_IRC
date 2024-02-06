#include "../includes/Bot.class.hpp"
#include "../includes/prt.hpp"
#include "../includes/swears.hpp"

Bot::Bot(string nickname, string realname):_connected(0), _botnick(nickname), _botname(nickname), _botreal(realname), _data_buffer(""){
	prt("BOT Created: " + nickname + "\n", BRIGHT_YELLOW);
	_forbiddenWords = getForbiddenWords();
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

	return 0;
}

void	Bot::sendMsg(const string &msg){
	string out = msg + "\r\n";
	send(_socket, out.c_str(), out.length(), 0);
	prt("<< " + msg + "\n", YELLOW);
}

s_user* Bot::createNewUser(const string& nick){
	s_user *user = new s_user;
	user->nick = nick;
	user->nb_warnings_forbidden = 0;
	user->last_warning_forbidden = 0;
	prt(nick + " new user create\n", BRIGHT_CYAN);
	return user;
}

