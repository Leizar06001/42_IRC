#include "../includes/server.class.hpp"
#include <sys/stat.h>	// MKDIR

void Server::writeToConLog(const string& str){
	_term.prtTmColor(str, Terminal::BRIGHT_CYAN);
	_logConStream << str;
	if (str[str.length() - 1] != '\n'){
		_logConStream << "\n";
		_logConStream.flush();
	}
}

void Server::writeToLog(const string& str){
	_logStream << str;
	if (str[str.length() - 1] != '\n'){
		_logStream << "\n";
		_logStream.flush();
	}
}

void Server::openLog(void){
	struct stat st;
	string path = LOG_FOLDER;
	if (stat(path.c_str(), &st) != 0) {
		// Directory does not exist.
		if (mkdir(path.c_str(), 0777) == -1) {
			_term.prtTmColor("ERROR: CANNOT CREATE LOG FOLDER\n", Terminal::BRIGHT_RED);
		}
	}
	_logStream.open((path + LOG_FILE).c_str(), std::fstream::out ); //| std::fstream::app
	if (!_logStream.is_open()) {
		_term.prtTmColor("ERROR: CANNOT OPEN LOG FILE\n", Terminal::BRIGHT_RED);
	} else {
		writeToLog(timestamp() + " Starting server..");
	}

	_logConStream.open((path + CON_LOG_FILE).c_str(), std::fstream::out ); //| std::fstream::app
	if (!_logConStream.is_open()) {
		_term.prtTmColor("ERROR: CANNOT OPEN CONNECTIONS LOG FILE\n", Terminal::BRIGHT_RED);
	} else {
		writeToConLog(timestamp() + " Starting server..");
	}
}

void Server::writeConf(const string& str){
	string path = CONF_FOLDER ;
	ofstream conf((path + CONF_FILE).c_str(), std::ios::out | std::ios::app);
	if (!conf.is_open()) {
		_term.prtTmColor("ERROR: CANNOT OPEN CONF FILE\n", Terminal::BRIGHT_RED);
	} else {
		conf << str << std::endl;
		conf.close();
		_term.prtTmColor("Configuration updated successfully.\n", Terminal::BRIGHT_GREEN);
	}
}

void Server::setVarsFromConf(string& str){
	vector<string> toks;
	size_t pos = str.find(" ");
	if (pos == string::npos || pos <= 0) return;
	string key = str.substr(0, pos);
	if (str.length() < pos + 1) return;
	string val = str.substr(pos + 1);

	if(key == "admin_pass") {
		if (val.length() == 64){
			_admin_password = val;
			_term.prtTmColor("sets to: " + val, Terminal::GREEN);
		}
		return ;
	}

	// _term.prtTmColor("key: " + key + "  val: " + val, Terminal::YELLOW);
	int num;
	num = atoi(val.c_str());
	if (num){
		if (key == "max_clients"){
			if (num > 0 && num < _max_fd_allowed){
				_max_clients = num;
				_term.prtTmColor("sets to: " + toString(num), Terminal::GREEN);
			} else _term.prtTmColor("default: " + toString(_max_clients), Terminal::YELLOW);
		} else if(key == "max_channels") {
			if (num > 0 && num < 10000){
				_max_channels = num;
				_term.prtTmColor("sets to: " + toString(num), Terminal::GREEN);
			} else _term.prtTmColor("default: " + toString(_max_channels), Terminal::YELLOW);
		} else if(key == "timeout_check_time") {
			if (num > 0 &&num < 120){
				_timeout_check_time = num;
				_term.prtTmColor("sets to: " + toString(num), Terminal::GREEN);
			} else _term.prtTmColor("default: " + toString(_timeout_check_time), Terminal::YELLOW);
		} else if(key == "connection_timeout") {
			if (num > 0 && num < 600){
				_connection_timeout = num;
				_term.prtTmColor("sets to: " + toString(num), Terminal::GREEN);
			} else _term.prtTmColor("default: " + toString(_connection_timeout), Terminal::YELLOW);
		} else if(key == "registration_timeout") {
			if (num > 0 && num < 120){
				_registration_timeout = num;
				_term.prtTmColor("sets to: " + toString(num), Terminal::GREEN);
			} else _term.prtTmColor("default: " + toString(_registration_timeout), Terminal::YELLOW);
		} else if(key == "max_users_chan") {
			if (num > 0 && num < 10000){
				_max_user_per_chan = num;
				_term.prtTmColor("sets to: " + toString(num), Terminal::GREEN);
			} else _term.prtTmColor("default: " + toString(_max_user_per_chan), Terminal::YELLOW);
		}
	}
}

void Server::readConf(void){
	struct stat st;
	string path = CONF_FOLDER;
	if (stat(path.c_str(), &st) != 0) {
		// Directory does not exist.
		if (mkdir(path.c_str(), 0777) == -1) {
			_term.prtTmColor("ERROR: CANNOT CREATE CONF FOLDER\n", Terminal::BRIGHT_RED);
		}
	}
	ifstream conf((path + CONF_FILE).c_str());
	if (!conf.is_open()) {
		_term.prtTmColor("ERROR: CANNOT OPEN CONF FILE\n", Terminal::BRIGHT_RED);
	} else {
		_term.prtTmColor("Reading conf file..\n", Terminal::CYAN);
		string line;
		while(getline(conf, line)){
			if (line == "[BANNED IP]"){
				_term.prtTmColor("BANNED\n", Terminal::MAGENTA);
				while(getline(conf, line)){
					if (line.empty() || line[0] == '[')
						break;
					if (line.length() < 20){
						_term.prtTmColor(line + "\n", Terminal::BRIGHT_RED);
						_bans_ip.push_back(line);
					}
				}
			} else {
				_term.prtTmColor(line + "\n", Terminal::MAGENTA);
				while(getline(conf, line)){
					if (line.empty() || line[0] == '[')
						break;
					_term.prtTmColor(line + "\n", Terminal::BLUE);
					setVarsFromConf(line);
				}
			}
		}
	}
}
