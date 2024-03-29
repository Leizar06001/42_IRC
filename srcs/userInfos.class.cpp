#include "../includes/userInfos.class.hpp"
#include <iostream>
#include "../includes/toString.hpp"
#include "../includes/terminal.class.hpp"
#include "../includes/ChannelList.class.hpp"

userInfos::userInfos(int fd, Terminal* term, int prt_debug):_term(term), _prt_debug(prt_debug){
	_registered = 0;
	_index = 0;
	_nb_msg = 0;
	_actionType = 0;
	_nickname_registered = 0;
	_username_registered = 0;
	_password = 0;
	_connection_start = time(NULL);
	_last_message = _connection_start;
	_waiting_for_pong = 0;
	_fd = fd;
	_nickname = "";
	_username = "";
	_realname = "";
	_status = "H";
	_is_invisible = 0;
	_is_admin = 0;
	_hide_host = 1;
	_ip_addr = "";
	_nb_wrong_cmds = 0;
	_incomming_msg = "";
	if (_prt_debug)
		_term->prtTmColor("New user created Fd #" + toString(fd) + "\n", Terminal::BLUE);
};
userInfos::~userInfos(void){
	if (_prt_debug)
		_term->prtTmColor("User " + _nickname + " destroyed\n", Terminal::BLUE);
};
userInfos::userInfos(userInfos & src){*this = src;};
userInfos& userInfos::operator=(const userInfos & src){
	if (this != &src){
		_registered = src._registered;
		_nickname_registered = src._nickname_registered;
		_username_registered = src._username_registered;
		_password = src._password;
		_connection_start = src._connection_start;
		_last_message = src._last_message;
		_waiting_for_pong = src._waiting_for_pong;
		_nickname = src._nickname;
		_prev_nick = src._prev_nick;
		_username = src._username;
		_realname = src._username;
		_ip_addr = src._ip_addr;
		_status = src._status;
		_is_invisible = src._is_invisible;
		_is_admin = src._is_admin;
		_hide_host = src._hide_host;
		_nb_msg = src._nb_msg;
		_fd = src._fd;
		_actionType = src._actionType;
		_term = src._term;
		_prt_debug = src._prt_debug;
		_nb_wrong_cmds = src._nb_wrong_cmds;
		_incomming_msg = src._incomming_msg;
	}
	return *this;
}

int userInfos::setNickname(string& nickname){
	_prev_nick = _nickname;
	_nickname = nickname;
	// if (_prt_debug)
	// 	_term->prtTmColor("FD." + toString(_fd) + " set nickname: " + nickname + "\n", Terminal::BLUE);
	if (!_registered)
		_nickname_registered = 1;
	else {
		//_actionType = ACT_CHANGED_NICK;
		return 1;
	}
	return 0;
}
int userInfos::setRealname(string& realname){
	if (!_registered){
		_realname = realname;
		// if (_prt_debug)
		// 	_term->prtTmColor("FD." + toString(_fd) + " " + _nickname + " set realname: " + realname + "\n", Terminal::BLUE);
		return 0;
	} else {
		// cout << "ERREUR REG" << endl;
		return 1;
	}
}
int userInfos::setUsername(string& username){
	if (!_registered){
		_username = username;
		// if (_prt_debug)
		// 	_term->prtTmColor("FD." + toString(_fd) + " " + _nickname + " set username: " + username + "\n", Terminal::BLUE);
		_username_registered = 1;
		return 0;
	} else {
		// cout << "ERREUR REG" << endl;
		return 1;
	}
}
void	userInfos::resetLastMessageTime(void){
	_last_message = time(NULL);
}
void	userInfos::setPong(bool ping){
	_waiting_for_pong = ping;
}
void userInfos::setIndex(int index){
	_index = index;
}
void	userInfos::incMsgs(void){
	++_nb_msg;
}
void	userInfos::incWrongCmds(void){
	++_nb_wrong_cmds;
}
void	userInfos::setIpAddr(string& add){
	_ip_addr = add;
}
void	userInfos::setStatus(const string& status){
	_status = status;
}
void	userInfos::setIncommingMsg(const string& msg){
	_incomming_msg = msg;
}
void	userInfos::setPasswordOk(void){
	_password = 1;
}
void	userInfos::setInvisible(bool invisible){
	_is_invisible = invisible;
}
void	userInfos::setAdmin(void){
	_is_admin = true;
}
void	userInfos::setHideHost(bool hide){
	_hide_host = hide;
}
void	userInfos::addChannelToList(s_Channel* chan){
	if (_channels.find(chan->channel_name) == _channels.end()){
		_channels.insert(pair<string, s_Channel*>(chan->channel_name, chan));
	}
}
void	userInfos::rmChannelFromList(s_Channel* chan){
	if (_channels.find(chan->channel_name) != _channels.end()){
		_channels.erase(chan->channel_name);
	}
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
string const userInfos::getUserMode(void) const{
	string mode = "+";
	if (_is_admin) mode += "Aa";
	if (_hide_host) mode += "x";
	if (_is_invisible) mode += "i";

	if (mode.length() <= 1) mode = "*";
	return mode;
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
time_t	userInfos::getLastMessageTime(void) const {
	return _last_message;
}
bool	userInfos::getPong(void) const {
	return _waiting_for_pong;
}
string const &userInfos::getIpAdress(void) const {
	return _ip_addr;
}
const string &userInfos::getStatus(void) const{
	return _status;
}
int userInfos::getWrongCmdsNb(void) const{
	return _nb_wrong_cmds;
}
string const &userInfos::getIncommingMsg(void) const {
	return _incomming_msg;
}
bool	userInfos::hasPassword(void) const {
	return _password;
}
bool	userInfos::isInvisible(void){
	return _is_invisible;
}
bool	userInfos::isAdmin(void){
	return _is_admin;
}
bool	userInfos::isHideHost(void){
	return _hide_host;
}

const map<string, s_Channel*>	*userInfos::getChannels(void) const {
	return &_channels;
}

int userInfos::checkReg(void) {
	if (!_registered && _username_registered && _nickname_registered && _password){
		_actionType = ACT_REGISTRATION;
		return 1;
	}
	return 0;
}

int userInfos::isRegistered(void) const {
	return _registered;
}
void userInfos::setRegistered(void) {
	_registered = 1;
}
