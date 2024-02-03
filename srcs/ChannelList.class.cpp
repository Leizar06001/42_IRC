#include "../includes/ChannelList.class.hpp"

ChannelList::ChannelList(Terminal* term):_term(term)
{
	s_Channel *general = new s_Channel;
	general->channel_name = "#General";
	general->channel_type = "Public";
	general->mode = "+nbt";
	general->deletable = 0;
	general->nb_users = 0;
	channel.insert(std::pair<std::string, s_Channel *>("#General", general));
	nb_channel = 1;
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
	if (channel_name[0] != '#')
	{
		_term->prtTmColor("Channel must begin with #", Terminal::RED);
		return 1;
	}
	std::map<std::string, s_Channel *>::iterator it = channel.find(channel_name);
	if (it != channel.end())
	{
		it->second->users.push_back(user);
		_term->prtTmColor("Channel " + channel_name + " exist", Terminal::BLUE);
		++it->second->nb_users;
	}
	else
	{
		s_Channel *new_channel = new s_Channel;
		new_channel->channel_name = channel_name;
		new_channel->channel_type = "Public";
		new_channel->mode = "+nbt";
		new_channel->deletable = 1;
		new_channel->nb_users = 1;
		channel.insert(std::pair<std::string, s_Channel *>(channel_name, new_channel));
		_term->prtTmColor("Channel " + channel_name + " created", Terminal::BLUE);
		new_channel->users.push_back(user);
		nb_channel++;
	}
	return 0;
}

void ChannelList::quitChannel(userInfos* user, std::string channel_name)
{
	std::map<std::string, s_Channel*>::iterator it = channel.find(channel_name);

	if (it != channel.end())
	{
		std::vector<userInfos*>::iterator it_u = it->second->users.begin();
		while (it_u != it->second->users.end())
		{
			if (*it_u == user){
				it_u = it->second->users.erase(it_u);
				--it->second->nb_users;
			}
			else
				++it_u;
		}
	}
}

void ChannelList::leaveServer(userInfos* user)
{
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
            }
            else
            {
                ++it_u;
            }
        }
		it++;
    }
}

void ChannelList::kickChannel(userInfos* user, std::string channel_name)
{
	std::map<std::string, s_Channel *>::iterator it = channel.find(channel_name);
	if (it != channel.end())
	{
		it->second->kicklist.push_back(user);
	}
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

std::string ChannelList::getUsersNicksInChan(string& channel_name)
{
	std::map<std::string, s_Channel *>::iterator it = channel.find(channel_name);
	std::string users;

	if(it != channel.end())
	{
		std::vector<userInfos *>::iterator it_u = it->second->users.begin();
		while(it_u != it->second->users.end())
		{
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

s_Channel* ChannelList::getNextChannel(int reset){
	static vector<s_Channel*>::iterator it;
	if (reset) it = channels.begin();
	if (it == channels.end()) return NULL;
	while (it != channels.end() && *it == NULL)
		++it;
	if (it == channels.end()) return NULL;
	s_Channel*	channel = *it;
	++it;
	return channel;
}
