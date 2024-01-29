#ifndef SERVER_CLASS_HPP
#define SERVER_CLASS_HPP

#include <string>
#include <netinet/in.h> // For sockaddr_in
#include <poll.h>

#define MAX_CON	100


class Server {

	private:
		int				_port;
		int 			_sockfd;
		sockaddr_in		_sockaddr;
		struct pollfd 	_fds[MAX_CON + 1];
		int				_msg_nb;
		int				_connection_nb;
		int				_initialized;


		std::string timestamp(void);

		int			pollFds(int timeout);
		void		getMessages(int fd);
		void		parseMessage(std::string& msg, int fd);


	public:
		Server(void);
		~Server(void);
		Server(Server &rhs);
		Server &operator=(Server &rhs);
		void	init(int port);
		void	shutdown(void);
		void	createSocket(int domain, int type, int protocol);
		void	bindToPort(int port);
		void	getConnection(void);

		void		handleEvents(void);

};




#endif
