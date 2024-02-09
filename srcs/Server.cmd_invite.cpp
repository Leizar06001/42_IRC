#include "../includes/server.class.hpp"

void Server::cmd_invite(int fd, vector<string> tokens)
{
	userInfos* user = _users->getUserByFd(fd);
	if (!user) return;
	if (tokens.size() < 3){ // Insufficient parameters
		sendServerMessage(fd, ERR_NEEDMOREPARAMS, "INVITE :Not enough parameters");
		return;
	}

	string nickname = tokens[1];
	string channel = tokens[2];

	userInfos* invitedUser = _users->getUserByNick(nickname);
	if (!invitedUser)
	{
		sendServerMessage(fd, ERR_NOSUCHNICK, nickname + " :No such nickname");
		return;
	}

	s_Channel* chan = _channels->getChannel(channel);
	if (!chan)
	{
		sendServerMessage(fd, ERR_NOSUCHCHANNEL, channel + " :No such channel");
		return;
	}

	// Check if the user sending the invite is actually in the channel
	if (!_channels->is_in_Channel(user, channel))
	{
		sendServerMessage(fd, ERR_NOTONCHANNEL, channel + " :You're not on that channel");
		return;
	}

	// Check if the invited user is already in the channel
	if (_channels->is_in_Channel(invitedUser, channel))
	{
		sendServerMessage(fd, ERR_USERONCHANNEL, nickname + " " + channel + " :is already on channel");
		return;
	}

	// Send invitation
	sendServerMessage(invitedUser->getFd(), RPL_INVITING, user->getNickname() + " " + nickname + " " + channel);
	sendServerMessage(fd, RPL_INVITING, nickname + " " + channel);

	// Add invitation to invited user's pending invitations
	_channels->inviteUser(invitedUser, channel);
}