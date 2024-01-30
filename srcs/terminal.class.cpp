#include "../includes/terminal.class.hpp"
#include "../includes/timestamp.hpp"

const std::string Terminal::RED = "\033[31m";
const std::string Terminal::GREEN = "\033[32m";
const std::string Terminal::YELLOW = "\033[33m";
const std::string Terminal::BLUE = "\033[34m";
const std::string Terminal::MAGENTA = "\033[35m";
const std::string Terminal::CYAN = "\033[36m";
const std::string Terminal::RESET = "\033[0m";

void Terminal::clearScreen() const {
	// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
	std::cout << "\033[2J\033[H";
}

void Terminal::clearLine(int row) const {
	setCursor(row, 0);
	std::cout << "\033[2K";
}

void Terminal::clearLineAfter(void) const {
	std::cout << "\033[0K";
}

// Set cursor position
void Terminal::setCursor(int row, int col) const {
	std::cout << "\033[" << row << ";" << col << "H";
}

// Print text in a specific color
void Terminal::prtColor(const std::string& text, const std::string& color) const {
	std::cout << color << text << RESET;
}

void Terminal::prtTmColor(const std::string& text, const std::string& color) const {
	std::cout << color << timestamp() << " " << text << RESET;
}

void Terminal::saveCursor(void) const {
	std::cout << "\033[s";
}

void Terminal::restoreCursor(void) const {
	std::cout << "\033[u";
}
