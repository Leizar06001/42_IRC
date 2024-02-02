#include "../includes/ChannelList.class.hpp"

ChannelList::ChannelList(Terminal* term):_term(term)
{
	s_Channel *general = new s_Channel;
	general->channel_name = "General";
	general->channel_type = "Public";
	general->deletable = 0;
	channel.insert(std::pair<std::string, s_Channel *>("General", general));
	_term->prtTmColor("Channel General created", Terminal::BLUE);
}

ChannelList::~ChannelList()
{
	size_t i = 0;
	while(i < channels.size())
	{
		if (channels[i])
		{
			delete channels[i];
		}
		i++;
	}
	channels.clear();
}

void ChannelList::joinChannel(userInfos* user, std::string channel_name)
{
	std::map<std::string, s_Channel *>::iterator it = channel.find(channel_name);
	int	is_channel = 0;
	if (it != channel.end())
	{
		it->second->users.push_back(user);
		is_channel++;
		_term->prtTmColor("Channel " + channel_name + " exist", Terminal::BLUE);
	}
	if(is_channel == 0)
	{
		s_Channel *new_channel = new s_Channel;
		new_channel->channel_name = channel_name;
		new_channel->channel_type = "Public";
		new_channel->deletable = 0;
		channel.insert(std::pair<std::string, s_Channel *>(channel_name, new_channel));
		_term->prtTmColor("Channel " + channel_name + " created", Terminal::BLUE);
		new_channel->users.push_back(user);
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

s_Channel	*ChannelList::getChannel(std::string channel_name)
{
	std::vector<s_Channel *>::iterator it = (this->channels).begin();
	while(it != channels.end())
	{
		if(((*it)->channel_name == channel_name))
		{
			return (*it);
		}
		it++;
	}
	return NULL;
}