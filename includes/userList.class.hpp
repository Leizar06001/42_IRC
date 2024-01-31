#ifndef USERLIST_CLASS_HPP
# define USERLIST_CLASS_HPP

#define MAX_USERS 100

#include <string>
#include "userInfos.class.hpp"
#include <vector>
#include <map>
#include "numerics.hpp"

using namespace std;

class Terminal;

class userList {

	private:
		vector<userInfos*>		_userlist;
		int						_nbUsers;
		map<int, size_t>		_mapID;		// userInfo Lookup by FD
		map<string, size_t>		_mapNick;	// userInfo Lookup by nickname
		map<int, size_t>		_mapInit;	// userInfo not registered yet
		map<int, size_t>		_mapAction;	// userInfo waiting for server action
		Terminal*				_term;

	public:
		userList(Terminal* term);
		~userList(void);
		userList(userList & src);
		userList &operator=(const userList & src);

		void	addUser(int fd);
		int		setNickname(int fd, string& nickname);
		int		setRealname(int fd, string& realname);
		int		setUsername(int fd, string& username);
		void		rmUser(int fd);

		int			getNbUsers(void);
		userInfos*	getUserByFd(int fd);
		userInfos*	getUserByNick(string& nickname);
		userInfos*	getNextUser(int reset);

		int			getNbNotRegistered(void) const;
		void		validateRegistration(userInfos* user);
		void		checkForRegistration(int fd);

		userInfos*	getUserActionRequests(void) const;
		void		rmFirstAction(void);
};

#endif
