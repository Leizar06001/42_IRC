#ifndef TERMINAL_CLASS_HPP
#define TERMINAL_CLASS_HPP

#include <iostream>
#include <string>
#include "userList.class.hpp"
#include <fstream>

using namespace std;

class userList;
class ChannelList;

class Terminal {

private:
	fstream* _logStream;

public:
	static const std::string BLACK;
	static const std::string RED;
	static const std::string GREEN;
	static const std::string YELLOW;
	static const std::string BLUE;
	static const std::string MAGENTA;
	static const std::string CYAN;
	static const std::string WHITE;
	static const std::string BRIGHT_BLACK;
	static const std::string BRIGHT_RED;
	static const std::string BRIGHT_GREEN;
	static const std::string BRIGHT_YELLOW;
	static const std::string BRIGHT_BLUE;
	static const std::string BRIGHT_MAGENTA;
	static const std::string BRIGHT_CYAN;
	static const std::string BRIGHT_WHITE;
	static const std::string RESET;

	Terminal(fstream* logStream);
	~Terminal(void);

	void updateTitle(int port, int clients, int not_reg, int channels, int msgs, int max_con, int max_chans, const vector<string> &bans_ip);
	void updateMenu(userList* users, ChannelList* channels);

	void clearScreen() const;
	void clearLine(int row) const;
	void clearLineAfter(void) const;
	void setCursor(int row, int col) const;
	void prtColor(const std::string& text, const std::string& color) const;
	void prtTmColor(const std::string& text, const std::string& color) const;
	void saveCursor(void) const;
	void restoreCursor(void) const;

};

#endif
