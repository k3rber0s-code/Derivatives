#include <iostream>
#include <string>
#include <vector>
#include "derivatives.h";
#include "postfix_convertor.h";

using namespace std;

bool CheckValidity(std::string expression) {
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	char variable;
	bool variableDefined = false;
	for (auto&& x : expression) {
		if (x == '(') {
			leftBracketCount++;
		}
		else if (x == ')') {
			rightBracketCount++;
		}
		else if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z')) {
			if (!variableDefined) {
				variable = x;
				variableDefined = true;
			}
			else {
				if (x != variable) {
					return false;
				}
			}
		}
		else if (x >= '0' && x <= '9') {
			continue;
		}
		else if (x == ' ' || x == '.') {
			continue;
		}
		else if (x == '+' || x == '-' || x == '/' || x == '*') {
			continue;
		}
		else {
			return false;
		}
	}
	if (leftBracketCount != rightBracketCount) {
		return false;
	}
	return true;

}
void ProcessExpression(string exp) {
	if (!CheckValidity(exp)) {
		cout << "Invalid input" << endl;
		return;
	}
	else {
		PostfixConvertor p;
		std::string converted_exp = p.InfixToPostfix(exp);
		if (converted_exp == "") {
			cout << "Invalid input" << endl;
			return;
		}

		ExpressionTree e(converted_exp);

		e.Differentiate(e.root);

		e.Simplify(e.root);

		e.TraverseInOrder(e.root);
	}
}
int main()
{
	//std::string exp = " 2 + 3 * x - 1 ";
	//std::string exp = " x / 0.5 + 4 * x ";
	//std::string exp = " x * x + 2 * x + 1 ";

	//std::string exp = "( 3 / x ) + ( 2 * x )";

	std::string exp = " 2 * x ";
	ProcessExpression(exp);


	return 0;
}