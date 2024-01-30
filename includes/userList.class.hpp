#ifndef USERLIST_CLASS_HPP
# define USERLIST_CLASS_HPP

#define MAX_USERS 100

#include <string>
#include "userInfos.class.hpp"
#include <vector>
#include <map>
#include "terminal.class.hpp"

using namespace std;

class userList {

	private:
		vector<userInfos*>		_userlist;
		int						_nbUsers;
		map<int, size_t>		_mapID;
		map<string, size_t>		_mapNick;
		map<int, size_t>		_mapInit;
		Terminal				_term;

	public:
		userList(void);
		~userList(void);
		userList(userList & src);
		userList &operator=(const userList & src);

		void	addUser(int fd);
		void	setNickname(int fd, string& nickname);
		void	setRealname(int fd, string& realname);
		void	setUsername(int fd, string& username);

		void		rmUser(int fd);
		userInfos*	getUserByFd(int fd);
		int			getNbNotRegistered(void) const;
};

#endif
