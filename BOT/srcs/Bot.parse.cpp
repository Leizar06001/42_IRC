#include "../includes/Bot.class.hpp"
#include "../includes/prt.hpp"
#include <sstream>

using namespace std;

vector<string>	Bot::parseNext(void){
	return parse(getNextMsg());
}

vector<string>	Bot::parse(const string &str){
	vector<string> 	tokens;
	string 			token;

	if (str.length() < 1) return tokens;

	// Split the message at ':' if exists
	size_t	colonPos = str.find(" :");
    string	beforeColon = (colonPos != string::npos) ? str.substr(0, colonPos) : str;
    string	afterColon = (colonPos != string::npos) ? str.substr(colonPos + 2) : "";

	// Split at spaces
	stringstream 	ss(beforeColon);
	while (std::getline(ss, token, ' ')) {
		if (!token.empty()) {
			tokens.push_back(token);
		}
	}
	// Push back after ':' part
	if (!afterColon.empty()) tokens.push_back(afterColon);

	if (tokens.size() > 0){
		prt(">> ", BRIGHT_CYAN);
		for(size_t i = 0; i < tokens.size(); ++i){
			switch (i){
				case 0: prt(tokens[i], MAGENTA); break;
				case 1: prt(tokens[i], CYAN); break;
				case 2: prt(tokens[i], GREEN); break;
				default: prt(tokens[i], WHITE); break;
			}
			cout << " ";
		}
		cout << endl;
	}
	return tokens;
}

const string Bot::getNextMsg(void){
	string msg = "";
	if (!_data_buffer.length()) return msg;

	size_t pos = 0;
	size_t start = 0;
	if ((pos = _data_buffer.find("\n", start)) != string::npos){	// FIND NEXT MESSAGES IN DATAS RECEIVED
		size_t msgLen = pos - start;
		if (pos > 0 && _data_buffer[pos - 1] == '\r')
			--msgLen;
		msg = _data_buffer.substr(start, msgLen);

		// prt("MSG: " + msg + "\n", BLUE);

		start = pos + 1;
	}
	// prt("Start: " + toString(start) + " buf len: " + toString(_data_buffer.length()) + "\n", WHITE);
	if (start < _data_buffer.length()){		// ADD FRAGMENTED DATA
		_data_buffer = _data_buffer.substr(start);
		// prt("Frag: " + _data_buffer + "\n", BRIGHT_RED);
	} else {
		_data_buffer = "";
	}
	return msg;
}
