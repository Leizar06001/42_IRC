#include "../includes/Bot.class.hpp"
#include "../includes/prt.hpp"

Bot::Bot(string nickname, string realname):_connected(0), _botnick(nickname), _botname(nickname), _botreal(realname), _data_buffer(""){
	prt("BOT Created: " + nickname + "\n", BRIGHT_YELLOW);
}
Bot::~Bot(void){
	close(_socket);
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

