#include <iostream>
#include <vector>

#include "Channel.hpp"
#include "terminal.class.hpp"

using namespace std;

class ChannelList
{
	private:
		vector<s_Channel *> channels;
		map<string, size_t>	channel_map;
		Terminal*	_term;

	public:
		ChannelList(Terminal* term);
		~ChannelList();
		void joinChannel(userInfos* user, string channel_name);
		void kickChannel(userInfos* user, string channel_name);
		s_Channel* getChannel(string channel_name);
};

int is_kicked(userInfos* user, s_Channel *channel);
