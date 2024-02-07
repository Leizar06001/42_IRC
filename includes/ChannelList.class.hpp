#include <iostream>
#include <vector>
#include <map>

#include "Channel.hpp"
#include "terminal.class.hpp"

using namespace std;

class ChannelList
{
	private:
		// vector<s_Channel *> channels;
		map<std::string, s_Channel *>	channels;
		Terminal*						_term;
		int 							nb_channel;
		int 							max_channel;
		int 							max_in_channel;


	public:
		ChannelList(Terminal* term);
		~ChannelList();
		int joinChannel(userInfos* user, string channel_name);

		int kickChannel(userInfos* kicker, userInfos* user, std::string channel_name);

		bool is_in_Channel(userInfos* user, string channel_name);
		bool is_operators(userInfos* user, string channel_name);

		s_Channel* getChannel(string& channel_name);
		string getUsersNicksInChan(string& chanel_name);

		void partChannel(userInfos* user, std::string channel_name);

		void quitServer(userInfos* user);

		int getNbChannel();
		int getNbUserInChannel();

		const string getUserPriviledges(const string& nick, const string& chan_name);
		const string getUserModes(userInfos* user, const string& chan_name);

		s_Channel* getNextChannel(int reset);

		void setMaxChannels(int max_channel);
		void setMaxInChannel(int max_in_channel);
};

// modes : n: no external messages, b: block users, t: mod topic, i: invite only = private ?, m: moderated, l: limited nb user
// channels   = : public, * : secret, @ : private
// users      @ : operator
