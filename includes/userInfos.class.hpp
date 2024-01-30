#ifndef USERINFOS_CLASS_HPP
# define USERINFOS_CLASS_HPP

#include <string>
#include "terminal.class.hpp"

using namespace std;

class userInfos {

	private:
		int			_index;
		int			_initialized;
		int			_registration_step;
		string		_nickname;
		string		_username;
		string		_realname;
		int			_fd;
		int			_nb_msg;
		Terminal	_term;

	public:
		userInfos(int fd);
		~userInfos(void);
		userInfos(userInfos & src);
		userInfos &operator=(const userInfos & src);

		void	setNickname(string& nickname);
		void	setUsername(string& username);
		void	setRealname(string& realname);
		void	setIndex(int index);

		void	incMsgs(void);

		string	getNickname(void) const;
		string	getUsername(void) const;
		string	getRealname(void) const;
		int		getFd(void) const;
		int		getNbMsg(void) const;
		int		getIndex(void) const;
};

#endif
