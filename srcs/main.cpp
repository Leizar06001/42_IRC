#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <string>
#include <signal.h>
#include <poll.h>
#include <sys/time.h>

#include "../includes/server.class.hpp"

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
	if (argc < 3){
		cout << "Error: Missing args { port, password }" << endl;
		return 1;
	}

	init_sigaction();

	Server serv;

	if (serv.init(atoi(argv[1]), string(argv[2])) == -1)
		return 1;
	while (!quitok)
		serv.handleEvents();


	return 0;
}

/* 	PASS password > ret: PASS ERR_PASSWDMISMATCH if wrong
	NICK nickname > :dan!d@localhost NICK Mamoped
	USER username 0 * (:)realname


*/
