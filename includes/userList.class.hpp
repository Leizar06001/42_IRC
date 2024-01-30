#ifndef USERLIST_CLASS_HPP
# define USERLIST_CLASS_HPP

#define MAX_USERS 100

#include <string>
#include "userInfos.class.hpp"
#include <vector>
#include <map>

using namespace std;

class userList {

	private:
		vector<userInfos*>		_userlist;
		int						_nbUsers;
		map<int, size_t>		_mapID;
		map<string, size_t>		_mapNick;

	public:
		userList(void);
		~userList(void);
		userList(userList & src);
		userList &operator=(const userList & src);

		void	addUser(int fd, string &nickname);

		void	rmUser(int fd);
		userInfos*	getUserByFd(int fd);
};

#endif
