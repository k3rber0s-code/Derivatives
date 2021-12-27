#pragma once
#include <iostream>;
class PostfixConvertor {
public:
	int GetPrecedence(std::string);
	void InfixToPostfix(std::string s);
};