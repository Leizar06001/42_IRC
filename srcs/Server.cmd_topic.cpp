#include "../includes/server.class.hpp"

bool hasTopicPermission(userInfos* user, s_Channel* channel)
{
    std::map<std::string, int>::iterator prefix_it = channel->prefix.find(user->getNickname());

    if (prefix_it != channel->prefix.end()){
        if (prefix_it->second >= 1 || channel->mode.find('t') == std::string::npos || user->isAdmin())
            return true;
		else
            return false;
    } else
        return false;
}

void Server::cmd_topic(int fd, vector<string> tokens)
{
    if (tokens.size() < 2){	// No channel
        return;
	}

	s_Channel* channel = _channels->getChannel(tokens[1]);
	if (!channel){
		sendServerMessage(fd, ERR_NOSUCHCHANNEL, tokens[1]);
		return;
	}
	userInfos* user = _users->getUserByFd(fd);
	if (!user)
		return;
	string topic = channel->topic;
	if (tokens.size() < 3){ 	// SEND TOPIC
		if (topic.empty())
			sendServerMessage(fd, RPL_NOTOPIC, tokens[1] + " :No topic is set");
		else
			sendServerMessage(fd, RPL_TOPIC, tokens[1] + " :" + topic);
		return ;
	}

	if (hasTopicPermission(user, channel)) {
		channel->topic = tokens[2];
		_term.prtTmColor("TOPIC Channel " + tokens[1] + " changed to " + tokens[2], Terminal::GREEN);
		//sendServerMessage(fd, RPL_TOPIC, tokens[1] + " :" + tokens[2]);
		sendServerMsgToList(fd, "332 " + user->getNickname() + " " + tokens[1] + " :" + tokens[2], channel->users);
	} else {
		sendServerMessage(fd, ERR_CHANOPRIVSNEEDED, user->getNickname() +  " :You're not channel operator");
	}
}


//void			sendServerMessage(int fd, int rpl_err_code, const string& msg); ERREUR
