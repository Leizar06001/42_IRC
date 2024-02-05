#include "../includes/prt.hpp"

// Print text in a specific color
void prt(const string& text, const string& color) {
	cout << color << text << RESET;
}
