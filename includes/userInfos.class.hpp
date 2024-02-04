#ifndef USERINFOS_CLASS_HPP
# define USERINFOS_CLASS_HPP

#include <string>
#include <ctime>
#include <map>

#define ACT_REGISTRATION	1
#define ACT_CHANGED_NICK	2

using namespace std;

typedef struct	t_Channel s_Channel;
class Terminal;

class userInfos {

	private:
		int			_index;				// index in the vector list
		int			_registered;		// correctly registered
		int			_nickname_registered;
		int			_username_registered;
		int			_password;
		time_t		_connection_start;
		time_t		_last_message;
		bool		_waiting_for_pong;
		string		_nickname;
		string		_prev_nick;
		string		_username;
		string		_realname;
		string		_ip_addr;
		string		_status;
		bool		_is_admin;		// * mode
		bool		_is_invisible;	// i
		bool		_hide_host;		// x
		int			_fd;
		int			_nb_msg;
		int			_actionType;		// tell the server what the client is waiting for
		Terminal*	_term;
		int			_prt_debug;	// print debug infos ?
		int			_nb_wrong_cmds;
		string		_incomming_msg;
		map<string, s_Channel*> _channels;


	public:
		userInfos(int fd, Terminal* term, int prt_debug);
		~userInfos(void);
		userInfos(userInfos & src);
		userInfos &operator=(const userInfos & src);

		int		setNickname(string& nickname);
		int		setUsername(string& username);
		int		setRealname(string& realname);
		void	setIndex(int index);
		void	resetLastMessageTime(void);
		void	setPong(bool ping);
		void	setIpAddr(string& add);
		void	setStatus(const string& status);
		void	setIncommingMsg(const string& msg);
		void	setPasswordOk(void);
		void	setInvisible(bool invisible);
		void	setAdmin(void);
		void	setHideHost(bool hide);
		void	addChannelToList(s_Channel* chan);
		void	rmChannelFromList(s_Channel* chan);

		void	incMsgs(void);
		void	incWrongCmds(void);

		string	const &getNickname(void) const;
		string	const &getPrevNick(void) const;
		string	const &getUsername(void) const;
		string	const &getRealname(void) const;
		string	const getUserMode(void) const;
		int		getFd(void) const;
		int		getNbMsg(void) const;
		int		getIndex(void) const;
		int		getAction(void) const;
		time_t	getConnectionStart(void) const;
		time_t	getLastMessageTime(void) const;
		bool	getPong(void) const;
		string	const &getIpAdress(void) const;
		string	const &getStatus(void) const;
		int		getWrongCmdsNb(void) const;
		string	const &getIncommingMsg(void) const;
		bool	hasPassword(void) const;
		bool	isInvisible(void);
		bool	isAdmin(void);
		bool	isHideHost(void);

		const map<string, s_Channel*>	*getChannels(void) const;

		int		checkReg(void);

		int		isRegistered(void) const;
		void	setRegistered(void);

};

#endif
