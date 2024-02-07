#include "../includes/timestamp.hpp"

std::string timestamp(void){
	time_t	t = time(NULL);
	tm* 	now = localtime(&t);
	char 	buf[16];
	std::strftime(buf, sizeof(buf), "%y%m%d-%H:%M:%S", now);
	std::string		str = toString(buf);
	return str;
}

std::string timestamp_fail2ban(void){
	time_t	t = time(NULL);
	tm* 	now = localtime(&t);
	char 	buf[30];
	std::strftime(buf, sizeof(buf), "%d-%b-%Y %H:%M:%S", now);
	std::string		str = toString(buf);
	return str;
}

