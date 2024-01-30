#ifndef USERINFOS_CLASS_HPP
# define USERINFOS_CLASS_HPP

#include <string>

using namespace std;

class userInfos {

	private:
		int		_initialized;
		string	_nickname;
		string	_username;
		string	_realname;
		int		_fd;

	public:
		userInfos(string& nickname);
		~userInfos(void);
		userInfos(userInfos & src);
		userInfos &operator=(const userInfos & src);

		void	setNickname(string& nickname);
		void	setUsername(string& username);
		void	setRealname(string& realname);
		void	setFd(int fd);

		string	getNickname(void);
		string	getUsername(void);
		string	getRealname(void);
		int		getFd(void) const;
};

#endif
