#include "../includes/userList.class.hpp"
#include <iostream>
#include <stdexcept>
#include "../includes/toString.hpp"
#include "../includes/terminal.class.hpp"
#include <ctime>

userList::userList(Terminal* term, int prt_debug):_nbUsers(0), _term(term), _prt_debug(prt_debug){
	if (_prt_debug)
		_term->prtTmColor("USER LIST CREATED\n", Terminal::MAGENTA);
};
userList::~userList(void){
	if (_prt_debug)
		_term->prtTmColor("User list size : " + toString(_userlist.size()) + "\n", Terminal::MAGENTA);
	for (size_t i = 0; i < _userlist.size(); ++i) {
		if (_userlist[i]){
			if (_prt_debug)
				_term->prtTmColor("deleting " + toString(i) + " : " + toString(_userlist[i]->getNickname()) + "\n", Terminal::MAGENTA);
			delete _userlist[i];
		}
	}
	_userlist.clear();
	if (_prt_debug)
		_term->prtTmColor("USER LIST DELETED\n", Terminal::MAGENTA);
};
userList::userList(userList & src){*this = src;};
userList& userList::operator=(const userList & src){
	(void)src;
	return *this;
}

userInfos* userList::addUser(int fd){
	if (_nbUsers >= MAX_USERS){
		throw length_error("USER DB FULL");
	}

	userInfos* user = new userInfos(fd, _term, _prt_debug);
	size_t index = 0;
	for (; index < _userlist.size(); ++index) {
		if (_userlist[index] == NULL) { // Found an empty slot
			_userlist[index] = user;
			if (_prt_debug)
				_term->prtTmColor("Inserted in vector: " + toString(index), Terminal::YELLOW);
			break;
		}
	}

	if (index == _userlist.size()) { // No empty slot found, add to the end
		_userlist.push_back(user);
		if (_prt_debug)
			_term->prtTmColor("Added end of vector " + toString(index), Terminal::YELLOW);
	}

	user->setIndex(index);
	_mapID.insert(make_pair(fd, index));
	_mapInit.insert(make_pair(fd, index));

	++_nbUsers;
	if (_prt_debug)
		_term->prtTmColor("FD." + toString(fd) + " User added at index " + toString(index) + "\n", Terminal::BRIGHT_CYAN);
	return user;
}

void userList::rmUser(int fd){
	// Remove user from maps
	map<int, size_t>::iterator itInit = _mapInit.find(fd);
	if (itInit != _mapInit.end()) _mapInit.erase(itInit);

	map<int, size_t>::iterator itAct = _mapAction.find(fd);
	if (itAct != _mapAction.end()) _mapAction.erase(itAct);

	map<int, size_t>::iterator itID = _mapID.find(fd);
	if (itID != _mapID.end()){
		size_t	index = itID->second;
		map<string, size_t>::iterator itNick = _mapNick.find(_userlist[index]->getNickname());
		if (itNick != _mapNick.end()) _mapNick.erase(itNick);
		if (_userlist[index] != NULL){
			delete _userlist[index];
			_userlist[index] = NULL;
		}
		if (_prt_debug){
			_term->prtTmColor("FD.'" + toString(fd) + "' deleted from vector index : " + toString(index) + "\n", Terminal::BRIGHT_BLUE);
			_term->prtTmColor("Removing FD " + toString(itID->first) + " index " + toString(itID->second) + " from mapID\n", Terminal::BRIGHT_BLUE);
		}
		_mapID.erase(itID);
		--_nbUsers;
	} else {
		_term->prtTmColor("FD.'" + toString(fd) + "' not found, cannot remove from mapID\n", Terminal::BRIGHT_BLUE);
	}
}

int userList::setNickname(int fd, string& nickname){
	string wrong_char = "@#/\\|=+-*%`~\'(): 0123456789";
	if (nickname.find_first_of(wrong_char) != string::npos)
		return ERR_ERRONEUSNICKNAME;
	string valid_nick = nickname;
	if (valid_nick.length() > 10)
		valid_nick = valid_nick.substr(0, 10);
	userInfos* check = getUserByNick(valid_nick);
	if (check)
		return ERR_NICKNAMEINUSE;
	userInfos* user = getUserByFd(fd);
	int ret = user->setNickname(valid_nick);
	if (ret >= 0){	// Worked
		if (ret == 1) {		// changed nick
			_mapNick.erase(user->getPrevNick());
		}
		_mapNick.insert(make_pair(valid_nick, user->getIndex()));
	}
	// if (user->isRegistered()){
	// 	_mapAction.insert(make_pair(fd, user->getIndex()));
	// }
	return ret;
}

int userList::setUsername(int fd, string& username){
	userInfos* user = getUserByFd(fd);
	if (user){
		return user->setUsername(username);
	}
	return -1;
}

int userList::setRealname(int fd, string& realname){
	userInfos* user = getUserByFd(fd);
	if (user){
		int ret = user->setRealname(realname);
		return ret;
	}
	return -1;
}

void userList::checkForRegistration(int fd){
	userInfos* user = getUserByFd(fd);
	if (user){
		if (user->checkReg() && !user->isRegistered()){
			validateRegistration(user);
			_mapAction.insert(make_pair(fd, user->getIndex()));
		}
	}
}

int userList::getNbUsers(void) const {
	return _nbUsers;
}

userInfos* userList::getUserByFd(int fd){
	map<int, size_t>::iterator it = _mapID.find(fd);
	if (it == _mapID.end()) return NULL;
	size_t	index = it->second;
	return _userlist[index];
}

userInfos* userList::getUserByNick(string& nickname){
	map<string, size_t>::iterator it = _mapNick.find(nickname);
	if (it == _mapNick.end()) return NULL;
	size_t	index = it->second;
	return _userlist[index];
}

userInfos* userList::getNextUser(int reset){
	static vector<userInfos*>::iterator it;
	if (reset) it = _userlist.begin();
	if (it == _userlist.end()) return NULL;
	while (it != _userlist.end() && *it == NULL)
		++it;
	if (it == _userlist.end()) return NULL;
	userInfos*	user = *it;
	++it;
	return user;
}

userInfos* userList::getUserByIndex(int index){
	userInfos* user;
	try {
		user = _userlist.at(index);
	} catch (std::out_of_range& e){
		user = NULL;
	}
	return user;
}

const string userList::getListOfUsers(void){
	map<string, size_t>::iterator it = _mapNick.begin();
	string	list;
	while (it != _mapNick.end()){
		list += it->first + " ";
		++it;
	}
	return list;
}

const map<int, size_t> &userList::getIDmap(void) const{
	return _mapID;
}

int userList::getNbNotRegistered(void) const {
	return _mapInit.size();
}

void userList::validateRegistration(userInfos* user){
	map<int, size_t>::iterator it = _mapInit.find(user->getFd());
	if (it != _mapInit.end()){
		_mapInit.erase(it);
		user->setRegistered();
	}
}

int userList::checkRegistrationTimeout(int timeout){
	map<int, size_t>::iterator it = _mapInit.begin();
	while (it != _mapInit.end()){
		if (time(NULL) - _userlist[it->second]->getConnectionStart() > timeout){
			return _userlist[it->second]->getFd();
		}
		++it;
	}
	return -1;
}

vector<userInfos*> userList::checkConnectionTimeout(int timeout){
	vector<userInfos*> list;
	vector<userInfos*>::iterator it = _userlist.begin();
	while (it != _userlist.end()){
		if ((*it)){
			if (time(NULL) - (*it)->getLastMessageTime() > timeout){
				list.push_back((*it));
			}
		}
		++it;
	}
	return list;
}

userInfos* userList::getUserActionRequests(void) const {
	if (!_mapAction.size())
		return NULL;
	else {
		map<int, size_t>::const_iterator it = _mapAction.begin();
		return _userlist[it->second];
	}
}

void userList::rmFirstAction(void){
	_mapAction.erase(_mapAction.begin());
}
