#include "../includes/userInfos.class.hpp"
#include <iostream>
#include "../includes/toString.hpp"

userInfos::userInfos(int fd){
	_initialized = 0;
	_registration_step = 0;
	_index = 0;
	_nb_msg = 0;
	_fd = fd;
	_nickname = "";
	_username = "";
	_realname = "";
	_term.prtTmColor("New user created Fd #" + toString(fd) + "\n", Terminal::BLUE);
};
userInfos::~userInfos(void){
	_term.prtTmColor("User " + _nickname + " destroyed\n", Terminal::BLUE);
};
userInfos::userInfos(userInfos & src){*this = src;};
userInfos& userInfos::operator=(const userInfos & src){
	if (this != &src){
		_initialized = src._initialized;
		_nickname = src._nickname;
		_username = src._username;
		_realname = src._username;
		_nb_msg = src._nb_msg;
		_fd = src._fd;
	}
	return *this;
}

void userInfos::setNickname(string& nickname){
	if (_registration_step == 0 || _initialized){
		_nickname = nickname;
		if (_registration_step == 0) ++_registration_step;
		_term.prtTmColor("FD.'" + toString(_fd) + " set nickname: " + nickname + "\n", Terminal::BLUE);
	} else {
		cout << "ERREUR REG" << endl;
	}
}
void userInfos::setRealname(string& realname){
	_realname = realname;
	_term.prtTmColor("FD.'" + toString(_fd) + " " + _nickname + " set realname: " + realname + "\n", Terminal::BLUE);
}
void userInfos::setUsername(string& username){
	if (_registration_step == 1 || _initialized){
		_username = username;
		_term.prtTmColor("FD.'" + toString(_fd) + " " + _nickname + " set username: " + username + "\n", Terminal::BLUE);
	} else {
		cout << "ERREUR REG" << endl;
	}
}
void userInfos::setIndex(int index){
	_index = index;
}
void	userInfos::incMsgs(void){
	++_nb_msg;
}

string userInfos::getNickname(void) const{
	return _nickname;
}
string userInfos::getUsername(void) const{
	return _username;
}
string userInfos::getRealname(void) const{
	return _realname;
}
int	userInfos::getFd(void) const {
	return _fd;
}
int userInfos::getNbMsg(void) const {
	return _nb_msg;
}
int userInfos::getIndex(void) const {
	return _index;
}
