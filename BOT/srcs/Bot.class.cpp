#include "../includes/Bot.class.hpp"
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cerrno>
#include <iostream> // For cout
#include <unistd.h> // For read
#include <arpa/inet.h>	//int_pton

Bot::Bot(string nickname, string realname):_botnick(nickname), _botname(nickname), _botreal(realname){
	cout << "BOT Created: " << nickname << endl;
}
Bot::~Bot(void){
	close(_socket);
	cout << "BOT CONNECTED" << endl;
}

int		Bot::connectTo(const string& host, int port, const string& pass){
	cout << "Initiating connection.." << endl;

	_port = port;
	_host = host;
	_pass = pass;

	// Create a socket
    int _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }
	cout << "..Socket created" << endl;

    // Set up server address structure
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, host.c_str(), &(serverAddr.sin_addr));

    // Connect to the server
    if (connect(_socket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to the server\n";
        close(_socket);
        return 1;
    }
	cout << "Connected to " << host << " " << port << endl;

	string msg = "PASS " + pass + "\r\n";
	send(_socket, msg.c_str(), msg.length(), 0);

	msg = "NICK " + _botnick + "\r\n";
	send(_socket, msg.c_str(), msg.length(), 0);

	msg = "USER " + _botname + " * * :" + _botreal + "\r\n";
	send(_socket, msg.c_str(), msg.length(), 0);

	return 0;
}

int		Bot::botLoop(void){
    char buffer[1024];

    // Assume sockfd is your socket descriptor

    ssize_t bytesRead = recv(_socket, buffer, sizeof(buffer), MSG_DONTWAIT);

    if (bytesRead == 0) {
        // Connection closed by the server
        cout << "Connection closed by the server\n";
    } else if (bytesRead < 0) {
        // Error handling for recv
        // cerr << "Error in recv\n";
    } else {
        // Data received
        // Process the received data
    }
	return 0;
}
