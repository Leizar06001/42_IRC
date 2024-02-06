#include "../includes/Bot.class.hpp"
#include "../includes/prt.hpp"

const string Bot::readSocket(void){
	string msg = "";

	char buffer[1024] = {0};

	if (poll(&_spollfd, 1, 50) == -1){
		cout << "Poll error" << endl;
	}

	if (_spollfd.revents & POLLIN){
		ssize_t bytesRead = recv(_socket, buffer, sizeof(buffer), MSG_DONTWAIT);

		if (bytesRead < 0 ){
			// ERROR
			if (errno != EAGAIN && errno != EWOULDBLOCK) {
				cout << "Recv err: " << strerror(errno) << endl;
				return msg;
			}
			// If EAGAIN or EWOULDBLOCK, simply no data available now, not an error


		} else if (bytesRead == 0){	// DISCONNECTED
			cout << "Connection lost" << endl;
			_connected = false;
			close(_socket);
			return msg;


		} else {				// TREAT MESSAGE
			string answer(buffer, bytesRead);
			_data_buffer += answer;
		}
	}
	return msg;
}
