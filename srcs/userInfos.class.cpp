#include "../includes/userInfos.class.hpp"
#include <iostream>
#include "../includes/toString.hpp"
#include "../includes/terminal.class.hpp"

userInfos::userInfos(int fd, Terminal* term):_term(term){
	_registered = 0;
	_registration_step = 0;
	_index = 0;
	_nb_msg = 0;
	_actionType = 0;
	_fd = fd;
	_nickname = "";
	_username = "";
	_realname = "";
	_term->prtTmColor("New user created Fd #" + toString(fd) + "\n", Terminal::BLUE);
};
userInfos::~userInfos(void){
	_term->prtTmColor("User " + _nickname + " destroyed\n", Terminal::BLUE);
};
userInfos::userInfos(userInfos & src){*this = src;};
userInfos& userInfos::operator=(const userInfos & src){
	if (this != &src){
		_registered = src._registered;
		_nickname = src._nickname;
		_username = src._username;
		_realname = src._username;
		_nb_msg = src._nb_msg;
		_fd = src._fd;
	}
	return *this;
}

int userInfos::setNickname(string& nickname){
	if (_registration_step == 0 || _registered){
		_nickname = nickname;
		if (_registration_step == 0) ++_registration_step;
		_term->prtTmColor("FD." + toString(_fd) + " set nickname: " + nickname + "\n", Terminal::BLUE);
		return 0;
	} else {
		// cout << "ERREUR REG" << endl;
		return 1;
	}
}
int userInfos::setRealname(string& realname){
	if (_registration_step == 2 || _registered){
		_realname = realname;
		_term->prtTmColor("FD." + toString(_fd) + " " + _nickname + " set realname: " + realname + "\n", Terminal::BLUE);
		++_registration_step;
		_actionType = ACT_REGISTRATION;
		return 0;
	} else {
		// cout << "ERREUR REG" << endl;
		return 1;
	}
}
int userInfos::setUsername(string& username){
	if (_registration_step == 1 || _registered){
		_username = username;
		if (_registration_step == 1) ++_registration_step;
		_term->prtTmColor("FD." + toString(_fd) + " " + _nickname + " set username: " + username + "\n", Terminal::BLUE);
		return 0;
	} else {
		// cout << "ERREUR REG" << endl;
		return 1;
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
int userInfos::getAction(void) const {
	return _actionType;
}

int userInfos::isRegistered(void) {
	return _registered;
}
void userInfos::setRegistered(void) {
	_registered = 1;
}
