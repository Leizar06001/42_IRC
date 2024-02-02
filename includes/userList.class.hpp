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
		int						_prt_debug;	// print debug infos ?

	public:
		userList(Terminal* term, int prt_debug);
		~userList(void);
		userList(userList & src);
		userList &operator=(const userList & src);

		void	addUser(int fd);
		int		setNickname(int fd, string& nickname);
		int		setRealname(int fd, string& realname);
		int		setUsername(int fd, string& username);
		void		rmUser(int fd);

		int			getNbUsers(void) const;
		userInfos*	getUserByFd(int fd);
		userInfos*	getUserByNick(string& nickname);
		userInfos*	getNextUser(int reset);
		userInfos*	getUserByIndex(int index);

		const string	getListOfUsers(void);
		const map<int, size_t> &getIDmap(void) const;

		int			getNbNotRegistered(void) const;
		void		validateRegistration(userInfos* user);
		void		checkForRegistration(int fd);
		int			checkRegistrationTimeout(int timeout);
		vector<userInfos*>	checkConnectionTimeout(int timeout);

		userInfos*	getUserActionRequests(void) const;
		void		rmFirstAction(void);
};

#endif
