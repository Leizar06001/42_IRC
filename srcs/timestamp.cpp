#include "../includes/timestamp.hpp"

std::string timestamp(void){
	time_t	t = time(NULL);
	tm* 	now = localtime(&t);
	char 	buf[16];
	std::strftime(buf, sizeof(buf), "%y%m%d-%H:%M:%S", now);
	std::string		str = toString(buf);
	return str;
}
