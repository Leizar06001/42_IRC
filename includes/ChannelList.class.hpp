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
		// doit retourner un code (int) si erreur (voir doc)

		void kickChannel(userInfos* user, string channel_name);
		// doit retourner un code (int) si erreur (voir doc)

		bool is_in_Channel(userInfos* user, string channel_name);
		s_Channel* getChannel(string& channel_name);
		string getUsersNicksInChan(string& chanel_name);

		void quitChannel(userInfos* user, std::string channel_name);
		// doit retourner un code (int) si erreur (voir doc)

		void leaveServer(userInfos* user);
		// doit retourner un code (int) si erreur (voir doc)

		int getNbChannel();
		s_Channel* getNextChannel(int reset);
};

// channels   = : public, * : secret, @ : private
	// users      @ : operator
