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
		int max_channel;
		int max_in_channel;



	public:
		ChannelList(Terminal* term);
		~ChannelList();
		int joinChannel(userInfos* user, string channel_name);
		// doit retourner un code (int) si erreur (voir doc) << c'est bon je pense >>
		// doit mettre a jour la map listant dans quelles channels le user est << c'est bon >>
		// verifier que le user n'est pas deja dans la channel (tu peux utiliser la map dans userinfos) << c'est bon >>

		int kickChannel(userInfos* kicker, userInfos* user, std::string channel_name);
		// doit retourner un code (int) si erreur (voir doc) << c'est bon >>

		bool is_in_Channel(userInfos* user, string channel_name);
		bool is_operators(userInfos* user, string channel_name);

		s_Channel* getChannel(string& channel_name);
		string getUsersNicksInChan(string& chanel_name);

		void partChannel(userInfos* user, std::string channel_name);
		// doit retourner un code (int) si erreur (voir doc)
		// supprimer channel si no user left
		// doit mettre a jour la map listant dans quelles channels le user est

		void quitServer(userInfos* user);
		// doit retourner un code (int) si erreur (voir doc)
		// supprimer channel si no user left
		// doit mettre a jour la map listant dans quelles channels le user est << c'est bon >>

		int getNbChannel();
		int getNbUserInChannel();

		const string getUserPriviledges(const string& nick, const string& chan_name);
		const string getUserModes(userInfos* user, const string& chan_name);

		s_Channel* getNextChannel(int reset);

		void setMaxChannel(int max_channel);
		void setMaxInChannel(int max_in_channel);
};

// modes : n: no external messages, b: block users, t: mod topic, i: invite only = private ?, m: moderated, l: limited nb user
// channels   = : public, * : secret, @ : private
// users      @ : operator
