#include "../includes/Bot.class.hpp"

void Bot::checkForForbiddenWords(const string& msg, s_user* user){
	string word = checkForbiddenWords(msg);
	if (word.length() > 0){
		string nick = user->nick;

			// Check if user exceeded warnings max
		if (user->nb_warnings_forbidden >= WARNINGS_BEFORE_KICK_FORBIDDEN){
			kickUser(user, "Your recent messages contain inappropriate language. We encourage respectful communication in this community.");
		} else {
			sendMsg("PRIVMSG #General :" + nick + " this kind of word is forbidden on this channel \"" + word + "\", you'll be kicked if you continue !");
		}

		prt("Forbidden word: " + word + "\n", BRIGHT_RED);

		// Add one warning
		++(user->nb_warnings_forbidden);
		user->last_warning_forbidden = time(NULL);
		prt(nick + " nb warnings: " + toString(user->nb_warnings_forbidden) + "\n", WHITE);
	}
}

void Bot::checkForSpam(s_user* user){
	string nick = user->nick;
	// Add message time to user's message list to check spam
	user->msg_times.push_back(time(NULL));
	if (user->msg_times.size() > NB_MESSAGE_SPAM){
		user->msg_times.erase(user->msg_times.begin());
	}

	// Check if user is spamming
	if (user->msg_times.size() == NB_MESSAGE_SPAM){
		if (time(NULL) - user->msg_times[0] < TIME_BEFORE_KICK_SPAM){
			// Spamming
			if (user->nb_warnings_spam >= WARNINGS_BEFORE_KICK_SPAM){
				kickUser(user, "You are spamming the channel. Please respect the community and avoid spamming.");
			} else {
				sendMsg("PRIVMSG #General :" + nick + " you are spamming the channel, you'll be kicked if you continue !");
				// reset user msg list
				user->msg_times.clear();
			}
			++(user->nb_warnings_spam);
		}
	}
}

void Bot::kickUser(s_user* user, const string& reason){
	string nick = user->nick;

	// Check if can reset kicks
	if (time(NULL) - user->last_kick > TIME_BEFORE_RESET_KICKS){
		user->nb_times_kicked = 0;
	}

	// Check if user exceeded kicks max
	if (user->nb_times_kicked >= NB_KICK_BEFORE_BAN){
		user->banned = true;
		sendMsg("KICK #General " + nick + " :YOU HAVE BEEN BANNED FOR " + toString(TIME_BEFORE_UNBAN) + "s " + reason);
		sendMsg("MODE #General +b " + nick);
		prt(nick + " banned\n", BRIGHT_RED);
		// Add to banned list
		_bannedUsers[nick] = time(NULL);
		return ;
	}

	sendMsg("KICK #General " + nick + " :" + reason);
	++user->nb_times_kicked;
	user->last_kick = time(NULL);
}
