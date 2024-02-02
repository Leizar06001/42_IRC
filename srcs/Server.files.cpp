#include "../includes/server.class.hpp"
#include <sys/stat.h>	// MKDIR

void Server::writeToLog(const string& str){
	_logStream << str;
	if (str[str.length() - 1] != '\n')
		_logStream << "\n";
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
	}
}

void Server::writeConf(const string& str){
	(void)str;
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
				_term.prtTmColor("CONF: Banned:\n", Terminal::MAGENTA);
				while(getline(conf, line)){
					if (line.empty() || line[0] == '[')
						break;
					if (line.length() < 20){
						_bans_ip.push_back(line);
						_term.prtTmColor(line + "\n", Terminal::MAGENTA);
					}
				}
			}
		}
	}
}
