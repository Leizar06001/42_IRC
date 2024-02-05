#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <string>
#include <signal.h>
#include <unistd.h>

#include "../includes/Bot.class.hpp"

using namespace std;

namespace {
	volatile sig_atomic_t quitok = false;
	void handle_break(int a) {
		if (a == SIGINT) quitok = true;
	}
}

void	init_sigaction(){
	struct	sigaction sigbreak;
	sigbreak.sa_handler = &handle_break;
	sigemptyset(&sigbreak.sa_mask);
	sigbreak.sa_flags = 0;
	if (sigaction(SIGINT, &sigbreak, NULL) != 0)
		std::cerr << "Error: sigaction\n";
}

int main(int argc, char **argv) {
	if (argc < 4){
		cout << "Error: Missing args { host, port, password }" << endl;
		return 1;
	}

	init_sigaction();

	Bot bot("RobotRock", "iRis Ope Bot");
	// Bot bot("local", "iRis Ope Bot");

	if (bot.connectTo(string(argv[1]), atoi(argv[2]), string(argv[3])) == -1)
		return 1;
	int stop = 0;
	while (!quitok && stop == 0){
		stop = bot.botLoop();
		usleep(1000);
	}


	return 0;
}

/* 	PASS password > ret: PASS ERR_PASSWDMISMATCH if wrong
	NICK nickname > :dan!d@localhost NICK Mamoped
	USER username 0 * (:)realname


*/
