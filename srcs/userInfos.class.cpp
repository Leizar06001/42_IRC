#include "../includes/userInfos.class.hpp"
#include <iostream>

userInfos::userInfos(string& nickname){
	_initialized = 0;
	_nickname = nickname;
	std::cout << "New user created: " << nickname << std::endl;
};
userInfos::~userInfos(void){
	std::cout << "User " << _nickname << " destroyed" << std::endl;
};
userInfos::userInfos(userInfos & src){*this = src;};
userInfos& userInfos::operator=(const userInfos & src){
	if (this != &src){
		_initialized = src._initialized;
		_nickname = src._nickname;
		_username = src._username;
		_realname = src._username;
		_fd = src._fd;
	}
	return *this;
}

void userInfos::setNickname(string& nickname){
	_nickname = nickname;
}
void userInfos::setRealname(string& realname){
	_realname = realname;
}
void userInfos::setUsername(string& username){
	_username = username;
	cout << "Nick: '" << _nickname << "' got Username: '" << _username << "'" << endl;
}
void	userInfos::setFd(int fd){
	_fd = fd;
}

string userInfos::getNickname(void){
	return _nickname;
}
string userInfos::getUsername(void){
	return _username;
}
string userInfos::getRealname(void){
	return _realname;
}
int	userInfos::getFd(void) const {
	return _fd;
}
