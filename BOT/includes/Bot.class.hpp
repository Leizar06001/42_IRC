#ifndef BOT_CLASS_HPP
# define BOT_CLASS_HPP

#include <string>
#include <poll.h>
#include <vector>
#include <iostream>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cerrno>
#include <iostream> // For cout
#include <unistd.h> // For read
#include <arpa/inet.h>	//int_pton
#include <cstring>
#include <set>
#include <map>
#include <ctime>

#include "prt.hpp"
#include "toString.hpp"

#define WARNINGS_BEFORE_KICK_FORBIDDEN	5
#define TIME_BEFORE_KICK_FORBIDDEN		120

#define NB_MESSAGE_SPAM					5
#define WARNINGS_BEFORE_KICK_SPAM		3
#define TIME_BEFORE_KICK_SPAM			10

#define TIME_BEFORE_RESET_KICKS			600
#define NB_KICK_BEFORE_BAN				1
#define TIME_BEFORE_UNBAN				600

using namespace std;

typedef struct s_user {
	string 			nick;
	int				nb_warnings_forbidden;
	time_t			last_warning_forbidden;
	int				nb_warnings_spam;
	vector<time_t>	msg_times;
	int 			nb_times_kicked;
	time_t			last_kick;
	bool			banned;
}				t_user;

class Bot {

	private:
		int 	_port;
		string	_host;
		string	_pass;
		int		_socket;
		pollfd	_spollfd;
		bool	_connected;

		string	_botnick;
		string	_botname;
		string	_botreal;

		string	_data_buffer;

		string	_servername;
		string	_nickOnServ;

		set<string> _forbiddenWords;

		map<string, s_user*>	_nickMap;
		map<string, time_t>		_bannedUsers;


		const string 	readSocket(void);
		const string	getNextMsg(void);
		vector<string>	parse(const string &str);
		vector<string>	parseNext(void);

		void			sendMsg(const string &msg);

		void	chooseCmd(const vector<string>& tokens);

		void	cmd_ping(const vector<string>& tokens);
		void	cmd_privmsg(const vector<string>& tokens);

		const string	checkForbiddenWords(string str);
		const string	getNickFromMsg(const string& str);

		s_user*	createNewUser(const string& nick);

		void	checkForForbiddenWords(const string& msg, s_user* user);
		void 	checkForSpam(s_user* user);
		void	kickUser(s_user* user, const string& reason);

	public:
		Bot(string nickname, string realname);
		~Bot(void);

		int	connectTo(const string& host, int port, const string& pass);
		int	botLoop(void);

};

#endif
