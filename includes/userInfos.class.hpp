#ifndef USERINFOS_CLASS_HPP
# define USERINFOS_CLASS_HPP

#include <string>

#define ACT_REGISTRATION 1

using namespace std;

class Terminal;

class userInfos {

	private:
		int			_index;				// index in the vector list
		int			_registered;		// correctly registered
		int			_registration_step;
		string		_nickname;
		string		_username;
		string		_realname;
		int			_fd;
		int			_nb_msg;
		int			_actionType;		// tell the server what the client is waiting for
		Terminal*	_term;

	public:
		userInfos(int fd, Terminal* term);
		~userInfos(void);
		userInfos(userInfos & src);
		userInfos &operator=(const userInfos & src);

		int		setNickname(string& nickname);
		int		setUsername(string& username);
		int		setRealname(string& realname);
		void	setIndex(int index);

		void	incMsgs(void);

		string	getNickname(void) const;
		string	getUsername(void) const;
		string	getRealname(void) const;
		int		getFd(void) const;
		int		getNbMsg(void) const;
		int		getIndex(void) const;
		int		getAction(void) const;

		int		isRegistered(void);
		void	setRegistered(void);

};

#endif
