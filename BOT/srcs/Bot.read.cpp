#include "../includes/Bot.class.hpp"
#include "../includes/prt.hpp"

const string Bot::readSocket(void){
	string msg = "";

	char buffer[1024] = {0};

	if (poll(&_spollfd, 1, 250) == -1){
		cout << "Poll error" << endl;
	}

	if (_spollfd.revents & POLLIN){
		ssize_t bytesRead = recv(_socket, buffer, sizeof(buffer), MSG_DONTWAIT);

		// string test = buffer;
		// prt(test, BLUE);

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
			return msg;
		} else {				// TREAT MESSAGE
			string answer(buffer, bytesRead);

			_data_buffer += answer;

			// answer = _data_buffer + answer;	// RETRIEVE PREVIOUS FRAGMENTED DATAS
			// size_t pos = 0;
			// size_t start = 0;
			// if ((pos = answer.find("\n", start)) != string::npos){	// FIND MESSAGES IN DATAS RECEIVED
			// 	size_t msgLen = pos - start;
			// 	if (pos > 0 && answer[pos - 1] == '\r')
			// 		--msgLen;
			// 	string msg = answer.substr(start, msgLen);

			// 	if (!msg.empty()){
			// 		// prt(msg + "\n", YELLOW);
			// 		parse(msg);
			// 	}

			// 	start = pos + 1;
			// }
			// if (start < answer.length()){		// ADD FRAGMENTED DATA
			// 	_data_buffer = answer.substr(start);
			// 	prt("Frag: " + _data_buffer, BRIGHT_RED);
			// }
		}
	}
	return msg;
}
