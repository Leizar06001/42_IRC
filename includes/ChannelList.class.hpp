#include <iostream>
#include <vector>
#include <map>

#include "Channel.hpp"
#include "terminal.class.hpp"

using namespace std;

class ChannelList
{
	private:
		vector<s_Channel *> channels;
		map<string, size_t>	channel_map;
		map<std::string, s_Channel *>	channel;
		Terminal*	_term;
		int nb_channel;

	public:
		ChannelList(Terminal* term);
		~ChannelList();
		int joinChannel(userInfos* user, string channel_name);
		void kickChannel(userInfos* user, string channel_name);
		bool is_in_Channel(userInfos* user, string channel_name);
		s_Channel* getChannel(string& channel_name);
		string getUsersNames(string& chanel_name);
		void quitChannel(userInfos* user, std::string channel_name);
		void leaveServer(userInfos* user);
		int getNbChannel();
};
