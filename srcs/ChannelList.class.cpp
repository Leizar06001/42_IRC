#include "../includes/ChannelList.class.hpp"

ChannelList::ChannelList(Terminal* term):_term(term)
{
	s_Channel *general = new s_Channel;
	general->channel_name = "General";
	general->channel_type = "Public";
	general->deletable = 0;
	(this->channel_map).insert(std::pair<std::string, size_t>("General", 0));
	(this->channels).push_back(general);
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
	std::vector<s_Channel *>::iterator it = (this->channels).begin();
	int	is_channel = 0;
	while(it != channels.end())
	{
		if(((*it)->channel_name == channel_name) && !is_kicked(user, (*it)))
		{
			(*it)->users.push_back(user);
			is_channel++;
			_term->prtTmColor("Channel " + channel_name + " exist", Terminal::BLUE);

		}
		it++;
	}
	if(is_channel == 0)
	{
		s_Channel *new_channel = new s_Channel;
		new_channel->channel_name = channel_name;
		new_channel->channel_type = "Public";
		new_channel->deletable = 0;
		(this->channel_map).insert(std::pair<std::string, size_t>(channel_name, 0));
		(this->channels).push_back(new_channel);
		_term->prtTmColor("Channel " + channel_name + " created", Terminal::BLUE);
		new_channel->users.push_back(user);
	}
}

void ChannelList::kickChannel(userInfos* user, std::string channel_name)
{
	std::vector<s_Channel *>::iterator it = (this->channels).begin();
	while(it != channels.end())
	{
		if(((*it)->channel_name == channel_name))
		{
			(*it)->kicklist.push_back(user);
		}
		it++;
	}
}

int	is_kicked(userInfos* user, s_Channel *channel)
{
	std::vector<userInfos *>::iterator it = channel->kicklist.begin();
	while(it != channel->kicklist.end())
	{
		if(((*it) == user))
		{
			std::cout << user->getUsername() << " is kicked" << std::endl;
			return(1);
		}
		it++;
	}
	std::cout << user->getUsername() << " is not kicked" << std::endl;
	return(0);
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
}
