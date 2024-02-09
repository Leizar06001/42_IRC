#include "../includes/Bot.class.hpp"
#include <cctype>

static std::string toLowerCase(const std::string& input) {
	std::string result = input;

	for (size_t i = 0; i < result.length(); ++i) {
		result[i] = std::tolower(result[i]);
	}

	return result;
}

const string	Bot::checkForbiddenWords(string str){
	str = toLowerCase(str);
	set<string>::iterator it = _forbiddenWords.begin();
	while (it != _forbiddenWords.end()){
		size_t pos = 0;
		size_t start = 0;
		while ((pos = str.find((*it), start)) != std::string::npos){
			// check if the word is not part of another word
			if ((pos == 0 || !isalpha(str[pos - 1])) && (pos + (*it).size() == str.size() || !isalpha(str[pos + (*it).size()]))){
				start = pos + 1;
				continue;
			}
			start = pos + 1;

			string ret((*it).size(), '*');
			ret[0] = (*it)[0];
			ret[(*it).size() - 1] = (*it)[(*it).size() - 1];
			return ret;
		}
		++it;
	}
	// if (ret.length() == 0) prt("MSG OK\n", GREEN);
	return "";
}
