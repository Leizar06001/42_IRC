#include "../includes/userList.class.hpp"
#include <iostream>
#include <stdexcept>

userList::userList(void):_nbUsers(0){
	cout << "USER LIST CREATED" << endl;
};
userList::~userList(void){
	cout << "User list size : " << _userlist.size() << endl;
	for (size_t i = 0; i < _userlist.size(); ++i) {
		if (_userlist[i]){
			cout << "deleting ";
			cout << i << endl;
			cout << _userlist[i]->getNickname() << endl;
			delete _userlist[i];
		}
	}
	_userlist.clear();
	cout << "USER LIST DELETED" << endl;
};
userList::userList(userList & src){*this = src;};
userList& userList::operator=(const userList & src){
	(void)src;
	return *this;
}

void userList::addUser(int fd, string &nickname){
	if (_nbUsers >= MAX_USERS){
		throw length_error("USER DB FULL");
	}
	userInfos* user = new userInfos(nickname);
	user->setFd(fd);
	_userlist.push_back(user);
	size_t	index = _userlist.size() - 1;
	_mapID.insert(make_pair(fd, index));
	_mapNick.insert(make_pair(nickname, index));

	++_nbUsers;
	cout << "User '" << nickname << "' added, fd: " << fd << std::endl;
}

void userList::rmUser(int fd){
	map<int, size_t>::iterator itID = _mapID.find(fd);
	if (itID != _mapID.end()){
		size_t	index = itID->second;
		map<string, size_t>::iterator itNick = _mapNick.find(_userlist[index]->getNickname());
		if (itNick != _mapNick.end()){
			_mapNick.erase(itNick);
			cout << "User fd '" << fd << "' removed from mapNick" << endl;
		} else {
			cout << "User fd '" << fd << "' not found, cannot remove from mapNick" << endl;
		}
		delete _userlist[index];
		_userlist[index] = NULL;
		cout << "User fd '" << fd << "' deleted from vector" << endl;
		_mapID.erase(itID);
		--_nbUsers;
		cout << "User fd '" << fd << "' removed from mapID" << endl;
	} else {
		cout << "User fd '" << fd << "' not found, cannot remove from mapID" << endl;
	}
}

userInfos* userList::getUserByFd(int fd){
	map<int, size_t>::iterator it = _mapID.find(fd);
	size_t	index = it->second;
	if (it != _mapID.end()){
		cout << "User fd '" << fd << "' found, Nick: " << _userlist[index]->getNickname() << endl;
	} else {
		cout << "User fd '" << fd << "' not found" << endl;
	}
	return _userlist[index];
}
