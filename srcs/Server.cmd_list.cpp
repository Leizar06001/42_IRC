#include "../includes/server.class.hpp"

void	Server::cmd_list(int fd, vector<string>){
	string msg;
	s_Channel* chan = _channels->getNextChannel(1);
	while (chan){
		const string type = chan->channel_type;
		if (type != "*"){
			msg = type + chan->channel_name + " " + toString(chan->nb_users) + " :" + chan->topic;
		}
		sendServerMessage(fd, RPL_LIST, msg);
		chan = _channels->getNextChannel(0);
	}
	sendServerMessage(fd, RPL_LISTEND, ":End of /LIST");
}

// chan types : = : public, * : secret, @ : private
