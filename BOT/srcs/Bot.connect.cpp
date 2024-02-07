#include "../includes/Bot.class.hpp"

int		Bot::connectTo(const string& host, int port, const string& pass){
	prt("Initiating connection..\n", BRIGHT_BLUE);

	_port = port;
	_host = host;
	_pass = pass;

	// Close the socket if it is already open
	if (_socket != -1){
		close(_socket);
	}

	// Create a new socket
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1) {
		prt( "Error creating socket\n", BRIGHT_RED);
		return -1;
	}
	prt("..Socket created\n", GREEN);

	// Set up server address structure
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, host.c_str(), &(serverAddr.sin_addr));

	// Connect to the server
	if (connect(_socket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
		prt("Error connecting to the server\n", BRIGHT_RED);
		close(_socket);
		return -1;
	}
	prt("Connected to " + host + " " + toString(port) + "\n", BRIGHT_GREEN);

	_spollfd.fd = _socket;
	_spollfd.events = POLLIN;
	_spollfd.revents = 0;

	_nickOnServ = _botnick;
	sendMsg("PASS " + pass);
	sendMsg("NICK " + _botnick);
	sendMsg("USER " + _botname + " * * :" + _botreal);

	int registered = 0;
	while (!registered){
		readSocket();
		vector<string> tokens;
		while ((tokens = parseNext()).size() > 0){
			// vector<string> tokens = parse(msg);
			if (tokens.size() >= 3){
				if (tokens[1] == "001"){		// WELCOME
					_nickOnServ = tokens[2];
					_servername = tokens[0].substr(1);
					registered = 1;
					prt("\n*** REGISTERED on server " + BRIGHT_MAGENTA + _servername + BRIGHT_GREEN + " ***\n\n", BRIGHT_GREEN);
					break;
				} else if (tokens[1] == "433"){		// NICK IN USER
					_nickOnServ += "_";
					sendMsg("NICK " + _nickOnServ);
				}
			}
			usleep(10000);
		}
		usleep(10000);
	}

	if (_botnick != _nickOnServ){
		prt("Bot nick has been changed to: " + _nickOnServ + "\n\n", BRIGHT_RED);
	}

	_connected = 1;

	return 0;
}
