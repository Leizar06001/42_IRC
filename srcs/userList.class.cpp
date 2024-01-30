#include "../includes/userList.class.hpp"
#include <iostream>
#include <stdexcept>
#include "../includes/toString.hpp"

userList::userList(void):_nbUsers(0){
	_term.prtTmColor("USER LIST CREATED\n", Terminal::MAGENTA);
};
userList::~userList(void){
	_term.prtTmColor("User list size : " + toString(_userlist.size()) + "\n", Terminal::MAGENTA);
	for (size_t i = 0; i < _userlist.size(); ++i) {
		if (_userlist[i]){
			_term.prtTmColor("deleting " + toString(i) + " : " + toString(_userlist[i]->getNickname()) + "\n", Terminal::MAGENTA);
			delete _userlist[i];
		}
	}
	_userlist.clear();
	_term.prtTmColor("USER LIST DELETED\n", Terminal::MAGENTA);
};
userList::userList(userList & src){*this = src;};
userList& userList::operator=(const userList & src){
	(void)src;
	return *this;
}

void userList::addUser(int fd){
	if (_nbUsers >= MAX_USERS){
		throw length_error("USER DB FULL");
	}
	userInfos* user = new userInfos(fd);
	_userlist.push_back(user);
	size_t	index = _userlist.size() - 1;
	user->setIndex(index);
	_mapID.insert(make_pair(fd, index));
	_mapInit.insert(make_pair(fd, index));

	++_nbUsers;
	_term.prtTmColor("FD." + toString(fd) + " User added\n", Terminal::MAGENTA);
}

void userList::setNickname(int fd, string& nickname){
	userInfos* user = getUserByFd(fd);
	user->setNickname(nickname);
	_mapNick.insert(make_pair(nickname, user->getIndex()));
}

void userList::setUsername(int fd, string& username){
	userInfos* user = getUserByFd(fd);
	user->setUsername(username);
}

void userList::setRealname(int fd, string& realname){
	userInfos* user = getUserByFd(fd);
	user->setRealname(realname);
}

void userList::rmUser(int fd){
	map<int, size_t>::iterator itID = _mapID.find(fd);
	if (itID != _mapID.end()){
		size_t	index = itID->second;
		map<string, size_t>::iterator itNick = _mapNick.find(_userlist[index]->getNickname());
		if (itNick != _mapNick.end()){
			_mapNick.erase(itNick);
			_term.prtTmColor("FD.'" + toString(fd) + "' removed from mapNick\n", Terminal::MAGENTA);
		} else {
			_term.prtTmColor("FD.'" + toString(fd) + "' not found, cannot remove from mapNick\n", Terminal::MAGENTA);
		}
		delete _userlist[index];
		_userlist[index] = NULL;
		_term.prtTmColor("FD.'" + toString(fd) + "' deleted from vector\n", Terminal::MAGENTA);
		_mapID.erase(itID);
		--_nbUsers;
		_term.prtTmColor("FD.'" + toString(fd) + "' removed from mapID\n", Terminal::MAGENTA);
	} else {
		_term.prtTmColor("FD.'" + toString(fd) + "' not found, cannot remove from mapID\n", Terminal::MAGENTA);
	}
}

userInfos* userList::getUserByFd(int fd){
	map<int, size_t>::iterator it = _mapID.find(fd);
	size_t	index = it->second;
	// if (it != _mapID.end()){
	// 	_term.prtTmColor("FD.'" + toString(fd) + "' found, Nick: " + _userlist[index]->getNickname() + "\n", Terminal::MAGENTA);
	// } else {
	// 	_term.prtTmColor("FD.'" + toString(fd) + "' not found\n", Terminal::MAGENTA);
	// }
	return _userlist[index];
}

int userList::getNbNotRegistered(void) const {
	return _mapInit.size();
}
