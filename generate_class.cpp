#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

std::string readFile(const std::string& fileName){
	std::ifstream file(fileName);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

std::string toUpper(const std::string& str){
	std::string upperStr = str;
	for (char& c : upperStr){
		c = std::toupper(static_cast<unsigned char>(c));
	}
	return upperStr;
}

std::string replace(const std::string& templateStr, const std::string& className){
	std::string result = templateStr;
	size_t startPos = 0;
	while ((startPos = result.find("ClassName", startPos)) != std::string::npos) {
		result.replace(startPos, 9, className);
		startPos += className.length();
	}
	startPos = 0;
	while ((startPos = result.find("CLASSNAME", startPos)) != std::string::npos) {
		result.replace(startPos, 9, toUpper(className));
		startPos += className.length();
	}
	return result;
}

void createFiles(const std::string& className){
	std::string hppTemplate = readFile("class_template.hpp");
	std::string cppTemplate = readFile("class_template.cpp");

	std::string hppContent = replace(hppTemplate, className);
	std::string cppContent = replace(cppTemplate, className);

	std::ofstream hppFile(className + ".hpp");
	hppFile << hppContent;
	hppFile.close();

	std::ofstream cppFile(className + ".cpp");
	cppFile << cppContent;
	cppFile.close();
}

int	main() {
	std::string className;
	std::cout << "Enter class name: ";
	std::cin >> className;

	createFiles(className);

	return 0;
}
