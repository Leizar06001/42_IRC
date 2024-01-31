#include "../includes/userInfos.class.hpp"
#include <iostream>
#include "../includes/toString.hpp"
#include "../includes/terminal.class.hpp"

userInfos::userInfos(int fd, Terminal* term):_term(term){
	_registered = 0;
	_index = 0;
	_nb_msg = 0;
	_actionType = 0;
	_nickname_registered = 0;
	_username_registered = 0;
	_connection_start = time(NULL);
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
	_prev_nick = _nickname;
	_nickname = nickname;
	_term->prtTmColor("FD." + toString(_fd) + " set nickname: " + nickname + "\n", Terminal::BLUE);
	if (!_registered)
		_nickname_registered = 1;
	else
		_actionType = ACT_CHANGED_NICK;
	return 0;
}
int userInfos::setRealname(string& realname){
	if (!_registered){
		_realname = realname;
		_term->prtTmColor("FD." + toString(_fd) + " " + _nickname + " set realname: " + realname + "\n", Terminal::BLUE);
		return 0;
	} else {
		// cout << "ERREUR REG" << endl;
		return 1;
	}
}
int userInfos::setUsername(string& username){
	if (!_registered){
		_username = username;
		_term->prtTmColor("FD." + toString(_fd) + " " + _nickname + " set username: " + username + "\n", Terminal::BLUE);
		_username_registered = 1;
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

string const &userInfos::getNickname(void) const{
	return _nickname;
}
string const &userInfos::getPrevNick(void) const{
	return _prev_nick;
}
string const &userInfos::getUsername(void) const{
	return _username;
}
string const &userInfos::getRealname(void) const{
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
time_t	userInfos::getConnectionStart(void) const {
	return _connection_start;
}

int userInfos::checkReg(void) {
	if (!_registered && _username_registered && _nickname_registered){
		_actionType = ACT_REGISTRATION;
		return 1;
	}
	return 0;
}

int userInfos::isRegistered(void) {
	return _registered;
}
void userInfos::setRegistered(void) {
	_registered = 1;
}
