#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include <string>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>
#include "userList.class.hpp"
#include "terminal.class.hpp"
#include <ctime>


#define MAX_CON					100
#define REGISTRATION_TIMEOUT	10
#define CONNECTION_TIMEOUT		60
#define TIMEOUT_CHECK_TIME		30
#define PRINT_DEBUG_INFOS		0

using namespace std;
class Server {

	private:
		string			_servername;
		int				_port;
		int 			_sockfd;
		sockaddr_in		_sockaddr;
		struct pollfd 	_fds[MAX_CON + 1];
		int				_msg_nb;
		int				_connection_nb;
		int				_initialized;
		userList*		_users;
		Terminal		_term;
		time_t			_last_timeout_check;

		void	createSocket(int domain, int type, int protocol);
		void	bindToPort(int port);
		void	getConnection(void);

		int				pollFds(int timeout);
		void		 	getMessages(int fd);
		vector<string>	parseMessage(int fd, string& msg);
		void			analyseCommands(int fd, vector<string>& tokens);
		void			sendMessage(int fd, const string& msg);

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
		void	cmd_whois(int fd, vector<string> tokens);
		void	cmd_names(int fd, vector<string> tokens);
		void	cmd_quit(int fd, vector<string> tokens);
		void	cmd_join(int fd, vector<string> tokens);

		void	rmUser(int fd, const string& reason);

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
