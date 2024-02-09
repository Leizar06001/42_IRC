#include <iostream>
#include <vector>
#include <map>

#include "userInfos.class.hpp"

typedef struct	t_Channel
{
	std::string						channel_name;
	std::string						channel_type;
	std::string						mode;
	std::map<string, userInfos *>	users;
	std::map<string, userInfos *>	operators;
	std::map<std::string, int>		prefix; // 0 = normal 1 = halfop 2 = operator 3 = protected(admin) 4 = founder
	std::map<string, userInfos *>	banlist;
	bool							deletable;
	std::string						topic;
	int								nb_users;
}				s_Channel;
