#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include <string>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>
#include "userList.class.hpp"
#include "terminal.class.hpp"


#define MAX_CON	100

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


		void	cmd_cap(int fd, vector<string> tokens);
		void	cmd_nick(int fd, vector<string> tokens);
		void	cmd_user(int fd, vector<string> tokens);
		void	cmd_ping(int fd, vector<string> tokens);
		void	cmd_msg(int fd, vector<string> tokens);
		void	cmd_who(int fd, vector<string> tokens);

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
