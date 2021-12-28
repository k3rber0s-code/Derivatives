#pragma once
#include <iostream>;
class PostfixConvertor {
public:
	int GetPrecedence(std::string);
	std::string InfixToPostfix(std::string s);
};