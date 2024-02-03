#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include <string>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>
#include "userList.class.hpp"
#include "ChannelList.class.hpp"
#include "terminal.class.hpp"
#include <ctime>
#include "toString.hpp"
#include "numerics.hpp"
#include <cerrno>
#include <sys/socket.h> // For socket functions
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream>
#include <unistd.h>
#include <fstream>

#define PRINT_DEBUG_INFOS		1

#define MAX_CLIENTS				10000
#define REGISTRATION_TIMEOUT	20
#define CONNECTION_TIMEOUT		120
#define TIMEOUT_CHECK_TIME		30
#define MAX_CHANNELS			100

#define CONF_FILE				"conf.txt"
#define CONF_FOLDER				"./conf/"
#define LOG_FILE				"last_log.txt"
#define LOG_FOLDER				"./log/"

using namespace std;
class Server {

	private:
		string			_servername;
		int				_max_fd_allowed;
		int				_port;
		int 			_sockfd;
		sockaddr_in		_sockaddr;
		struct pollfd 	_fds[MAX_CLIENTS + 1];
		int				_msg_nb;
		int				_connection_nb;
		int				_initialized;
		userList*		_users;
		ChannelList*	_channels;

		time_t			_last_timeout_check;
		fstream			_logStream;
		Terminal		_term;

		vector<string>	_bans_ip;
		int				_max_clients;
		int				_max_channels;
		int				_timeout_check_time;
		int				_connection_timeout;
		int				_registration_timeout;


		void	createSocket(int domain, int type, int protocol);
		void	bindToPort(int port);
		void	getConnection(void);

		int				pollFds(int timeout);
		void		 	getMessages(int fd);
		vector<string>	parseMessage(int fd, string& msg);
		void			analyseCommands(int fd, vector<string>& tokens);
		void			sendMessage(int fd, const string& msg);
		void			sendClientMessage(int fd, const string& msg);
		void			sendClientMessageShowIp(int fd, const string& msg);
		void			sendServerMessage(int fd, int rpl_err_code, const string& msg);

		void	performAction(userInfos* user);
		void	drawInterface(void);
		void	checkClientRegistrationTimeout(void);
		void	checkIfClientsAlive(void);
		void	readSockets(void);
		void	checkPendingActions(void);

		void	cmd_cap(int fd, vector<string> tokens);
		void	cmd_nick(int fd, vector<string> tokens);
		void	cmd_user(int fd, vector<string> tokens);
		void	cmd_ping(int fd, vector<string> tokens);
		void	cmd_pong(int fd, vector<string> tokens);
		void	cmd_msg(int fd, vector<string> tokens);
		void	cmd_who(int fd, vector<string> tokens);
		void	cmd_whois(int fd, vector<string> tokens);
		void	cmd_names(int fd, vector<string> tokens);
		void	cmd_quit(int fd, vector<string> tokens);
		void	cmd_join(int fd, vector<string> tokens);
		void	cmd_kick(int fd, vector<string> tokens);
		void	cmd_invite(int fd, vector<string> tokens);
		void	cmd_topic(int fd, vector<string> tokens);
		void	cmd_mode(int fd, vector<string> tokens);
		void	cmd_part(int fd, vector<string> tokens);
		void	cmd_ban(int fd, vector<string> tokens);
		void	cmd_userhost(int fd, vector<string> tokens);
		void	cmd_notice(int fd, vector<string> tokens);

		void	rmUser(int fd, const string& reason);
		void	forceDisconnect(int fd, const string& reason);

		void	writeToLog(const string& str);
		void	writeConf(const string& str);
		void	readConf(void);
		void	setVarsFromConf(string &str);
		void	openLog(void);

		int		isIPBanned(const string& ip);
		void	addToBannedList(const string& ip);
		bool	isBotTraffic(const string &str);
		void	sendMsgToList(int fd_source, const string& msg, const map<int, size_t> &lst);
		void	sendMsgToList(int fd_source, const string& msg, vector<userInfos*> lst);

	public:
		Server(void);
		~Server(void);
		Server(Server &rhs);
		Server &operator=(Server &rhs);

		int		init(int port);
		void	shutdown(void);
		void	handleEvents(void);
};

// Fermer fd apres timeout si pas registered
// Refaire le systeme de registration (accepter infos dans le desordre)


#endif
