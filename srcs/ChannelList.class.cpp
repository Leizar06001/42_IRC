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
	channel.insert(std::pair<std::string, s_Channel *>("#General", general));
	nb_channel = 1;
	max_channel = 100;
	max_in_channel = 100;
}

ChannelList::~ChannelList()
{
	std::map<std::string, s_Channel*>::iterator it = channel.begin();
	while (it != channel.end())
	{
		if (it->second)
		{
			delete it->second;
		}
		++it;
	}
	channel.clear();
}

int ChannelList::joinChannel(userInfos* user, std::string channel_name)
{
	// ****** LISTE DES ERREURS ******
	// ERR_TOOMANYCHANNELS ERR_BADCHANMASK ERR_BANNEDFROMCHAN ERR_CHANNELISFULL ERR_INVITEONLYCHAN

	if(nb_channel > max_channel)
	{
		// _term->prtTmColor("Maximum number of Channel is" + toString(max_channel), Terminal::RED);
		return ERR_TOOMANYCHANNELS;
	}
	if (channel_name[0] != '#')
	{
		// _term->prtTmColor("Channel must begin with #", Terminal::RED);
		return ERR_BADCHANMASK;
	}
	std::map<std::string, s_Channel *>::iterator it = channel.find(channel_name);
	if(!is_in_Channel(user, channel_name))
	{
		if (it != channel.end())	// CHANNEL EXISTS
		{
			if(it->second->nb_users > max_in_channel)
			{
				_term->prtTmColor("Maximum number of client in Channel is" + toString(max_in_channel), Terminal::RED);
				return 471;
			}
			it->second->users.push_back(user);
			if (user->isAdmin())
				it->second->prefix.insert(std::pair<std::string, int>(user->getNickname(), 2));
			else
				it->second->prefix.insert(std::pair<std::string, int>(user->getNickname(), 0));
			_term->prtTmColor("Channel " + channel_name + " exist", Terminal::BLUE);
			++it->second->nb_users;
			user->addChannelToList(it->second);
		}
		else						// NEW CHANNEL
		{
			s_Channel *new_channel = new s_Channel;
			new_channel->channel_name = channel_name;
			new_channel->channel_type = "=";
			new_channel->mode = "+mntlb";	// mod, no msg from out, topic protect, limit, ban
			new_channel->deletable = 1;
			new_channel->nb_users = 1;
			channel.insert(std::pair<std::string, s_Channel *>(channel_name, new_channel));
			_term->prtTmColor("Channel " + channel_name + " created", Terminal::BLUE);
			new_channel->users.push_back(user);
			new_channel->operators.push_back(user);
			new_channel->prefix.insert(std::pair<std::string, int>(user->getNickname(), 2));
			nb_channel++;
			user->addChannelToList(new_channel);
		}
	}
	return 0;
}

void ChannelList::partChannel(userInfos* user, std::string channel_name)
{
	if (!user) return;
	std::map<std::string, s_Channel*>::iterator it = channel.find(channel_name);

	if (it != channel.end())
	{
		std::vector<userInfos*>::iterator it_u = it->second->users.begin();
		while (it_u != it->second->users.end())
		{
			if (*it_u == user){
				it_u = it->second->users.erase(it_u);
				--it->second->nb_users;
				if(it->second->nb_users == 0)    //suprimer le chan
				{
					channel_map.erase(channel_name);
					channel.erase(channel_name);
					return;
				}
			}
			else
				++it_u;
		}
	} else {
		_term->prtTmColor("PART: Channel not found " + channel_name, Terminal::RED);
	}
}

void ChannelList::quitServer(userInfos* user)
{
	if (!user) return;
    std::map<std::string, s_Channel*>::iterator it = channel.begin();

    while (it != channel.end())
    {
        std::vector<userInfos*>::iterator it_u = it->second->users.begin();
        while (it_u != it->second->users.end())
        {
            if (*it_u == user)
            {
                it_u = it->second->users.erase(it_u);
				--it->second->nb_users;
				if(it->second->nb_users == 0)    //suprimer le chan
				{
					if(it->second->deletable == 1)
					{
						channel_map.erase(it->first);
						channel.erase(it->first);
						return;
					}
				}
            }
            else
            {
                ++it_u;
            }
        }
		it++;
    }
}

int ChannelList::kickChannel(userInfos* kicker, userInfos* user, std::string channel_name)
{
	if (!user || !kicker) return -1;

	// Check if chan exists
	std::map<std::string, s_Channel *>::iterator it = channel.find(channel_name);
	if (it == channel.end())
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
	std::map<std::string, s_Channel *>::iterator it = channel.find(channel_name);
	if (it != channel.end())
	{
		return (it->second);
	}
	return NULL;
}

bool ChannelList::is_in_Channel(userInfos* user, string channel_name)
{
	std::map<std::string, s_Channel *>::iterator it = channel.find(channel_name);
	if(it != channel.end())
	{
		std::vector<userInfos *>::iterator it_u = it->second->users.begin();
		while(it_u != it->second->users.end())
		{
			if((*it_u) == user)
				return(1);
			it_u++;
		}
	}
	return(0);
}

bool ChannelList::is_operators(userInfos* user, string channel_name)
{
	std::map<std::string, s_Channel *>::iterator it = channel.find(channel_name);
	if(it != channel.end())
	{
		std::vector<userInfos *>::iterator it_u = it->second->operators.begin();
		while(it_u != it->second->operators.end())
		{
			if((*it_u) == user)
				return(1);
			it_u++;
		}
	}
	return(0);
}

std::string ChannelList::getUsersNicksInChan(string& channel_name)
{
	std::map<std::string, s_Channel *>::iterator it = channel.find(channel_name);
	std::string users;

	if(it != channel.end())
	{
		std::vector<userInfos *>::iterator it_u = it->second->users.begin();
		while(it_u != it->second->users.end())
		{
			if (it->second->prefix.find((*it_u)->getNickname())->second == 4)
			{
				users += "~";		// FOUNDER
			}
			else if (it->second->prefix.find((*it_u)->getNickname())->second == 3)
			{
				users += "&";		// PROTECTED
			}
			else if (it->second->prefix.find((*it_u)->getNickname())->second == 2)
			{
				users += "@";		// OPE
			}
			else if (it->second->prefix.find((*it_u)->getNickname())->second == 1)
			{
				users += "%";		// HALF OP
			}
			users += (*it_u)->getNickname();
			users += " ";
			it_u++;
		}
	}
	return users;
}

int ChannelList::getNbChannel()
{
	return(this->nb_channel);
}

const string ChannelList::getUserPriviledges(const string& nick, const string& chan_name){
	// FIND CHANNEL
	map<string, s_Channel*>::iterator it = channel.find(chan_name);
	if (it == channel.end()) return "";
	// FIND USER IN CHANNEL
	map<string, int>::iterator itt = it->second->prefix.find(nick);
	if (itt == it->second->prefix.end()) return "";
	switch (itt->second){
		case 1: return "%";	// half op
		case 2: return "@";	// op
		case 3: return "@";	// protect	Should be &
		case 4: return "@";	// founder	Should be ~
		default: return "";
	}
	return "";
}

const string ChannelList::getUserModes(userInfos* user, const string& chan_name){
	string modes = user->getUserMode();
	// FIND CHANNEL
	map<string, s_Channel*>::iterator it = channel.find(chan_name);
	if (it == channel.end()) return "";
	// FIND USER IN CHANNEL
	map<string, int>::iterator itt = it->second->prefix.find(user->getNickname());
	if (itt == it->second->prefix.end()) return "";
	switch (itt->second){
		case 1: modes += "h";	// half op
		case 2: modes += "o";	// op
		case 3: modes += "p";	// protect
		case 4: modes += "q";	// founder
	}

	return modes;
}

s_Channel* ChannelList::getNextChannel(int reset){
	static map<string, s_Channel*>::iterator it;
	if (reset) it = channel.begin();
	if (it == channel.end()) return NULL;
	while (it != channel.end() && it->second == NULL)
		++it;
	if (it == channel.end()) return NULL;
	s_Channel*	chan = it->second;
	++it;
	return chan;
}

void ChannelList::setMaxChannel(int max_channel)
{
	this->max_channel = max_channel;
}

void ChannelList::setMaxInChannel(int max_in_channel)
{
	this->max_in_channel = max_in_channel;
}


