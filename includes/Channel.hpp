#include <iostream>
#include <vector>
#include <map>

#include "userInfos.class.hpp"

typedef struct	t_Channel
{
	std::string						channel_name;
	std::string						channel_type;
	std::string						mode;
	std::vector<userInfos *>		users;
	std::vector<userInfos *>		kicklist;
	bool							deletable;
	std::string						topic;
}				s_Channel;
