#ifndef TERMINAL_CLASS_HPP
#define TERMINAL_CLASS_HPP

#include <iostream>
#include <string>

class Terminal {
public:
	// ANSI color codes
	static const std::string RED;
	static const std::string GREEN;
	static const std::string YELLOW;
	static const std::string BLUE;
	static const std::string MAGENTA;
	static const std::string CYAN;
	static const std::string RESET;

	// Clear the screen
	void clearScreen() const {
		// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
		std::cout << "\033[2J\033[H";
	}

	void clearLine(int row) const {
		setCursor(row, 0);
		std::cout << "\033[2K";
	}

	void clearLineAfter(void) const {
		std::cout << "\033[0K";
	}

	// Set cursor position
	void setCursor(int row, int col) const {
		std::cout << "\033[" << row << ";" << col << "H";
	}

	// Print text in a specific color
	void prtColor(const std::string& text, const std::string& color) const {
		std::cout << color << text << RESET;
	}

	void saveCursor(void) const {
		std::cout << "\033[s";
	}

	void restoreCursor(void) const {
		std::cout << "\033[u";
	}
};



#endif
