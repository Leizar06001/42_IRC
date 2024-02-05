#ifndef BOT_CLASS_HPP
# define BOT_CLASS_HPP

#include <string>

using namespace std;

class Bot {

	private:
		int 	_port;
		string	_host;
		string	_pass;
		int		_socket;

		string	_botnick;
		string	_botname;
		string	_botreal;


	public:
		Bot(string nickname, string realname);
		~Bot(void);

		int	connectTo(const string& host, int port, const string& pass);
		int	botLoop(void);

};

#endif
