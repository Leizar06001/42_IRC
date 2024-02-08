#include "../includes/ChannelList.class.hpp"
#include "../includes/toString.hpp"

ChannelList::ChannelList(Terminal* term):_term(term)
{
	s_Channel *general = new s_Channel;
	general->channel_name = "#General";
	general->channel_type = "=";
	general->mode = "+mntlb";
	general->deletable = 0;
	general->nb_users = 0;
	// channel.insert(std::pair<std::string, s_Channel *>("#General", general));
	channels["#General"] = general;
	nb_channel = 1;
	max_channel = 100;
	max_in_channel = 100;
}

ChannelList::~ChannelList()
{
	std::map<std::string, s_Channel*>::iterator it = channels.begin();
	while (it != channels.end())
	{
		if (it->second)
			delete it->second;
		++it;
	}
	channels.clear();
}

int ChannelList::joinChannel(userInfos* user, std::string channel_name)
{
	// ****** LISTE DES ERREURS ******
	// ERR_TOOMANYCHANNELS ERR_BADCHANMASK ERR_BANNEDFROMCHAN ERR_CHANNELISFULL ERR_INVITEONLYCHAN

	if(nb_channel > max_channel)
		return ERR_TOOMANYCHANNELS;
	if (channel_name[0] != '#')
		return ERR_BADCHANMASK;
	string wrong_char = "@&$#/\\|=+-*%`~\'\"(): ";
	if (channel_name.substr(1).find_first_of(wrong_char) != string::npos)
		return ERR_BADCHANMASK;

	std::map<std::string, s_Channel *>::iterator it = channels.find(channel_name);
	if(!is_in_Channel(user, channel_name))
	{
		if (it != channels.end())	// CHANNEL EXISTS
		{
			if(it->second->nb_users > max_in_channel)
				return ERR_CHANNELISFULL;

			it->second->users[user->getNickname()] = user;
			if (user->isAdmin())
				it->second->prefix.insert(std::pair<std::string, int>(user->getNickname(), 2));
			else
				it->second->prefix.insert(std::pair<std::string, int>(user->getNickname(), 0));
			// _term->prtTmColor("Channel " + channel_name + " exist", Terminal::BLUE);
			++it->second->nb_users;
			user->addChannelToList(it->second);
		}
		else						// NEW CHANNEL
		{
			// check if name is valid
			string wrong_char = " :&%^@!*\"\t\n\r";
			if (wrong_char.find(channel_name[0]) != string::npos)
				return ERR_BADCHANMASK;
			if (channel_name.length() > 15)
				return ERR_INPUTTOOLONG;

			// CREATE NEW CHANNEL
			s_Channel *new_channel = new s_Channel;
			new_channel->channel_name = channel_name;
			new_channel->channel_type = "=";
			new_channel->mode = "+mntlb";	// mod, no msg from out, topic protect, limit, ban
			new_channel->deletable = 1;
			new_channel->nb_users = 1;
			channels.insert(std::pair<std::string, s_Channel *>(channel_name, new_channel));
			_term->prtTmColor("Channel " + channel_name + " created", Terminal::BRIGHT_BLUE);
			new_channel->users[user->getNickname()] = user;
			new_channel->operators.insert(make_pair<string, userInfos*>(user->getNickname(), user));
			new_channel->prefix.insert(std::pair<std::string, int>(user->getNickname(), 2));
			nb_channel++;
			user->addChannelToList(new_channel);
		}
	}
	return 0;
}

int ChannelList::partChannel(userInfos* user, std::string channel_name)
{
    if (!user) return;
    std::map<std::string, s_Channel*>::iterator itchan = channels.find(channel_name);

    if (itchan == channels.end()){
		_term->prtTmColor("PART: Channel not found " + channel_name, Terminal::RED);
		return ERR_NOSUCHCHANNEL;
	}

	std::map<string, userInfos*>::iterator ituser = itchan->second->users.find(user->getNickname());
	if (ituser == itchan->second->users.end())	// find user in channel user list
		return ERR_NOTONCHANNEL;					// User not in this channel

	itchan->second->users.erase(ituser);	// remove user from channel's user list
	--itchan->second->nb_users;
	if(itchan->second->nb_users <= 0 && itchan->second->deletable)    //suprimer le chan si vide
	{
		delete itchan->second;
		channels.erase(itchan);
		nb_channel--;
		return 0;
	}

	// Delete user from this channel's prefix map
	itchan->second->prefix.erase(user->getNickname());
	// Delete user from this channel's operator map
	itchan->second->operators.erase(user->getNickname());
	return 0;
}

void ChannelList::quitServer(userInfos* user)
{
    if (!user) return;
    std::map<std::string, s_Channel*>::iterator it_chan = channels.begin();

	// Create a vector of all the chans to remove
	vector<s_Channel*> chans_user_is_in;
    while (it_chan != channels.end())	// For each channel, check if user is in it
    {
		if (it_chan->second->users.find(user->getNickname()) != it_chan->second->users.end())
			chans_user_is_in.push_back(it_chan->second);
		++it_chan;
    }

	// Remove user from all the chans he's in
	for (size_t i = 0; i < chans_user_is_in.size(); ++i){
		partChannel(user, chans_user_is_in[i]->channel_name);
	}
}

int ChannelList::kickChannel(userInfos* kicker, userInfos* user, std::string channel_name)
{
	if (!user || !kicker) return -1;

	// Check if chan exists
	std::map<std::string, s_Channel *>::iterator it = channels.find(channel_name);
	if (it == channels.end())
	{
		_term->prtTmColor("KICK error, The Channel doesn't exist " + channel_name, Terminal::YELLOW);
		return ERR_NOSUCHCHANNEL;
	}

	// Case if the kicker is SERVER ADMIN, can do anything he wants
	if (kicker->getUserMode().find("A") != std::string::npos){
		partChannel(user, channel_name);
		_term->prtTmColor("User " + Terminal::BRIGHT_YELLOW + user->getNickname() + Terminal::BRIGHT_RED + " has been kicked from " + channel_name, Terminal::BRIGHT_RED);
		return 0;
	}

	map<string, int>::iterator priv_kicker = it->second->prefix.find(kicker->getNickname());		// Get kicker priviledges
	if (priv_kicker != it->second->prefix.end()){													// kicker is in the map

		map<string, int>::iterator priv_target = it->second->prefix.find(user->getNickname());		// Get target priviledges
		if(priv_kicker->second > 0 && priv_kicker->second > priv_target->second) 					// Is not a simple user & has more priviledges than target
		{

			// OK FOR KICK
			partChannel(user, channel_name);
			_term->prtTmColor("User " + Terminal::BRIGHT_YELLOW + user->getNickname() + Terminal::BRIGHT_RED + " has been kicked from " + channel_name, Terminal::BRIGHT_RED);
			return 0;

			// it->second->kicklist.push_back(user);		?? une ban list ?
		}
	}

	// If we get here > The kicker doesn't have enough priviledges to kick the target
	_term->prtTmColor("KICK error, The user don't have enough privilege " + kicker->getNickname(), Terminal::YELLOW);
	return ERR_CHANOPRIVSNEEDED;

	// ****************************************
	// AJOUTER ERR_USERNOTINCHANNEL		(user in not in this channel)
	//
	// ****************************************
}

s_Channel	*ChannelList::getChannel(std::string& channel_name)
{
	std::map<std::string, s_Channel *>::iterator it = channels.find(channel_name);
	if (it != channels.end())
	{
		return (it->second);
	}
	return NULL;
}

bool ChannelList::is_in_Channel(userInfos* user, string channel_name)
{
	std::map<std::string, s_Channel *>::iterator it = channels.find(channel_name);
	if(it != channels.end())
	{
		if(it->second->users.find(user->getNickname()) != it->second->users.end())
			return(1);
	}
	return(0);
}

bool ChannelList::is_operators(userInfos* user, string channel_name)
{
	std::map<std::string, s_Channel *>::iterator it = channels.find(channel_name);
	if(it != channels.end())
	{
		std::map<std::string, userInfos *>::iterator it_u = it->second->operators.find(user->getNickname());
		if(it_u != it->second->operators.end())
			return(1);
	}
	return(0);
}

std::string ChannelList::getUsersNicksInChan(string& channel_name)
{
	std::map<std::string, s_Channel *>::iterator itchan = channels.find(channel_name);
	std::string users = "";

	if(itchan == channels.end())
		return users;

	std::map<string, userInfos *>::iterator it_u = itchan->second->users.begin();
	while(it_u != itchan->second->users.end())
	{
		int user_grade = itchan->second->prefix.find(it_u->second->getNickname())->second;
		switch (user_grade){
			case 1: users += "%"; break;	// half op
			case 2: users += "@"; break;	// op
			case 3: users += "@"; break;	// protect
			case 4: users += "@"; break;	// founder
		}

		users += it_u->second->getNickname();
		users += " ";
		it_u++;
	}

	return users;
}

int ChannelList::getNbChannel()
{
	return(this->nb_channel);
}

const string ChannelList::getUserPriviledges(const string& nick, const string& chan_name){
	// FIND CHANNEL
	map<string, s_Channel*>::iterator it = channels.find(chan_name);
	if (it == channels.end()) return "";
	// FIND USER IN CHANNEL
	map<string, int>::iterator itt = it->second->prefix.find(nick);
	if (itt == it->second->prefix.end()) return "";
	switch (itt->second){
		case 1: return "%";	// half op
		case 2: return "@";	// op
		case 3: return "@";	// protect	Should be &
		case 4: return "@";	// founder	Should be ~
	}
	return "";
}

const string ChannelList::getUserModes(userInfos* user, const string& chan_name){
	string modes = user->getUserMode();
	// FIND CHANNEL
	map<string, s_Channel*>::iterator it = channels.find(chan_name);
	if (it == channels.end()) return "";
	// FIND USER IN CHANNEL
	map<string, int>::iterator itt = it->second->prefix.find(user->getNickname());
	if (itt == it->second->prefix.end()) return "";
	switch (itt->second){
		case 1: modes += "h"; break;	// half op
		case 2: modes += "o"; break;	// op
		case 3: modes += "p"; break;	// protect
		case 4: modes += "q"; break;	// founder
	}

	return modes;
}

s_Channel* ChannelList::getNextChannel(int reset){
	static map<string, s_Channel*>::iterator it;
	if (reset) it = channels.begin();
	if (it == channels.end()) return NULL;
	while (it != channels.end() && it->second == NULL)
		++it;
	if (it == channels.end()) return NULL;
	s_Channel*	chan = it->second;
	++it;
	return chan;
}

void ChannelList::setMaxChannels(int max_channel)
{
	this->max_channel = max_channel;
}

void ChannelList::setMaxInChannel(int max_in_channel)
{
	this->max_in_channel = max_in_channel;
}


