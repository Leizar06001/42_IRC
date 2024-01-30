#ifndef TERMINAL_CLASS_HPP
#define TERMINAL_CLASS_HPP

#include <iostream>
#include <string>

class Terminal {
public:
	static const std::string RED;
	static const std::string GREEN;
	static const std::string YELLOW;
	static const std::string BLUE;
	static const std::string MAGENTA;
	static const std::string CYAN;
	static const std::string RESET;

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
