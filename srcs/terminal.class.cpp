#include "../includes/terminal.class.hpp"
#include "../includes/timestamp.hpp"
#include "../includes/userList.class.hpp"
#include "../includes/userInfos.class.hpp"

#define MENU_W	25
#define TITLE_H	10
#define WIN_H	50

const std::string Terminal::BLACK = "\033[30m";
const std::string Terminal::RED = "\033[31m";
const std::string Terminal::GREEN = "\033[32m";
const std::string Terminal::YELLOW = "\033[33m";
const std::string Terminal::BLUE = "\033[34m";
const std::string Terminal::MAGENTA = "\033[35m";
const std::string Terminal::CYAN = "\033[36m";
const std::string Terminal::WHITE = "\033[37m";
const std::string Terminal::BRIGHT_BLACK = "\033[90m";
const std::string Terminal::BRIGHT_RED = "\033[91m";
const std::string Terminal::BRIGHT_GREEN = "\033[92m";
const std::string Terminal::BRIGHT_YELLOW = "\033[93m";
const std::string Terminal::BRIGHT_BLUE = "\033[94m";
const std::string Terminal::BRIGHT_MAGENTA = "\033[95m";
const std::string Terminal::BRIGHT_CYAN = "\033[96m";
const std::string Terminal::BRIGHT_WHITE = "\033[97m";
const std::string Terminal::RESET = "\033[0m";

Terminal::Terminal(fstream* logStream):_logStream(logStream){};

Terminal::~Terminal(void){};

void Terminal::updateMenu(userList* users){
	// saveCursor();
	(void)users;

	for(int i = TITLE_H + 1; i < WIN_H; ++i){
		setCursor(i, 1);
		prtColor("▞", Terminal::BLUE);
		setCursor(i, 100 - MENU_W);
		if (i % 2)
			prtColor("╽", Terminal::BLUE);
		else
			prtColor("╿", Terminal::BLUE);
		clearLineAfter();
		setCursor(i, 100);
		prtColor("▞", Terminal::BLUE);
	}
	int h = TITLE_H + 2;
	const int x = 100 - MENU_W + 1;

	setCursor(h, x + 2);
	prtColor("_.~\"( CHANNELS )\"~._", Terminal::BRIGHT_MAGENTA);

	h += 2;
	setCursor(h, x + 3);
	prtColor("_.~\"( USERS  )\"~._", Terminal::BRIGHT_GREEN);
	++h;
	int ind = 0;
	const int nb_users = users->getNbUsers();
	userInfos*	user = users->getNextUser(1);
	while ((h < WIN_H - 1) && (ind < nb_users)){
		if (user){
			setCursor(h, x + 4);
			if (user->isRegistered())
				prtColor("◎ " + user->getNickname() + " :" + toString(user->getFd()), Terminal::WHITE);
			else
				prtColor("! Fd" + toString(user->getFd()), Terminal::RED);
			++h;
			++ind;
		}
		user = users->getNextUser(0);
	}

	// restoreCursor();
	setCursor(WIN_H - 1, 3);
	cout << flush;
}

void Terminal::updateTitle(int port, int clients, int not_reg, int channels, int msgs, int max_con){
	static int dec = 0;
	// saveCursor();
	for(int i = 1; i <= TITLE_H + 1; ++i){
		clearLine(i);
	}
	setCursor(4, dec);
	prtColor(">>    >>    >>    >>    >>    >>    >>    >>    >>    >>    >>    >>    >>    >>    >>    >>    >>", Terminal::BLUE);
	clearLine(5);
	for(int i = 1; i <= TITLE_H; ++i){
		setCursor(i, 1);
		prtColor("▞", Terminal::BLUE);
		setCursor(i, 100);
		prtColor("▞", Terminal::BLUE);
		clearLineAfter();
	}
	setCursor(1, 1);
	prtColor("■▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰■", Terminal::BLUE);
	setCursor(TITLE_H, 1);
	prtColor("■▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰┯▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰■", Terminal::BLUE);
	setCursor(WIN_H, 1);
	prtColor("■▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰┷▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰■", Terminal::BLUE);

	// TITLE
	setCursor(4, 35);
	prtColor("\033[1m (ﾟ◥益◤ﾟ) i̲̭̘̊ͣR͔̅ͨi͉̝͈͓s̝̍ͩ͌̀̉.̹͙͗͐C̦̠h͉̯a̦̝̙͔ͯͭͫͬț̰̓ͦ́ͤ̈  (ʘ言ʘ╬) ", Terminal::CYAN);
	// LEFT SIDE
	setCursor(6, 10);
	prtColor("   Port: " + toString(port), Terminal::BRIGHT_CYAN);
	setCursor(7, 10);
	prtColor("Clients: " + toString(clients) + "/" + toString(max_con), Terminal::BRIGHT_GREEN);
	if (not_reg) {
		setCursor(7, 26);
		prtColor("! " + toString(not_reg) + " not reg", Terminal::RED);
	}
	// RIGHT SIDE
	setCursor(6, 75);
	prtColor("Channels: " + toString(channels), Terminal::MAGENTA);
	setCursor(7, 75);
	prtColor("Messages: " + toString(msgs), Terminal::MAGENTA);

	// // COLOR TESTS
	// setCursor(7, 82);
	// prtColor("███", Terminal::BLUE);
	// prtColor("███", Terminal::CYAN);
	// prtColor("███", Terminal::GREEN);
	// prtColor("███", Terminal::MAGENTA);
	// prtColor("███", Terminal::RED);
	// prtColor("███", Terminal::YELLOW);

	// setCursor(8, 82);
	// prtColor("███", Terminal::BRIGHT_BLUE);
	// prtColor("███", Terminal::BRIGHT_CYAN);
	// prtColor("███", Terminal::BRIGHT_GREEN);
	// prtColor("███", Terminal::BRIGHT_MAGENTA);
	// prtColor("███", Terminal::BRIGHT_RED);
	// prtColor("███", Terminal::BRIGHT_YELLOW);

	// restoreCursor();
	cout << flush;	// update display
	++dec;
	if (dec > 6) dec = 0;
}

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
	clearLine(WIN_H);
	setCursor(WIN_H - 1, 3);
	string tm = timestamp();

	size_t visibleCount = 0;
	size_t i = 0;
	if (text.length() < 56 && text.find("\033") == string::npos)
		std::cout << color << tm << " " << text << RESET;
	else {

		while (i < text.length() && visibleCount < 55) {
			if (text[i] == '\033' && i + 1 < text.length() && text[i + 1] == '[') {
				i += 2; // Skip '\033['
				while (i < text.length() && text[i] != 'm')
					i++;
				if (i < text.length())
					i++; // Skip 'm'
			} else {
				visibleCount++;
				i++;
			}
		}
		std::cout << color << tm << " " << text.substr(0, i);
		if (text.substr(i).length() > 0){
			std::cout << "\033[S";	// scroll up
			setCursor(WIN_H - 1, 19);
			std::cout << text.substr(i);
		}
	}

	// Write to log
	*_logStream << tm << " " << text;
	if (text[text.length() - 1] != '\n')
		*_logStream << "\n";
	_logStream->flush();

	// std::cout << "\033[S"; 	// scroll up
	// setCursor(WIN_H - 1, 3);
	// cout << "i: " << i << " vis: " << visibleCount;

	std::cout << "\033[S"; 	// scroll up
	setCursor(WIN_H, 1);
	prtColor("■▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰┷▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰▰■", Terminal::BLUE);
	setCursor(WIN_H - 1, 3);
	std::cout << flush;
}

void Terminal::saveCursor(void) const {
	std::cout << "\033[s";
}

void Terminal::restoreCursor(void) const {
	std::cout << "\033[u";
}
