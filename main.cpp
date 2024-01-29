#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <string>
#include <signal.h>
#include <poll.h>
#include <sys/time.h>

#include "includes/server.class.hpp"

std::string username;
std::string nickname;



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
		std::perror("Error: sigaction\n");
}

int main() {
	init_sigaction();

	Server serv;

	serv.init(6667);
	while (!quitok)
		serv.handleEvents();


	return 0;
}
