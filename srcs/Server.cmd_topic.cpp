#include "../includes/server.class.hpp"

bool hasTopicPermission(userInfos* user, s_Channel* channel)
{
    std::map<std::string, int>::iterator prefix_it = channel->prefix.find(user->getNickname());

    if (prefix_it != channel->prefix.end())
	{
        int userPrefix = prefix_it->second;

        if (userPrefix <= 2)
		{
            // Les founder (0), les protected (1) et les operators (2) ont le droit de modifier le topic
            return true;
        }
		else
		{
            return false;
        }
    }
	else
	{
        return false;
    }
}

void Server::cmd_topic(int fd, vector<string> tokens)
{
    if (tokens.size() < 2)
	{
        return;
    }

    string channel_name = tokens[1];
    string topic;
    s_Channel* channel = _channels->getChannel(channel_name);

    if (channel)
	{
		if (hasTopicPermission(_users->getUserByFd(fd), channel))
		{
			for (size_t i = 2; i < tokens.size(); ++i)
			{
				topic += tokens[i];
				if (i < tokens.size() - 1)
				{
					topic += " ";
				}
			}
			channel->topic = topic;

			sendMsgToList(fd, "TOPIC " + channel_name + " :" + topic, _users->getIDmap());
		}
		else
		{
            sendServerMessage(fd, 482, "You're not channel operator");
        }

	}
}


//void			sendServerMessage(int fd, int rpl_err_code, const string& msg); ERREUR