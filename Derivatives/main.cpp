#include <iostream>
#include <string>
#include <vector>
#include "derivatives.h";
#include "postfix_convertor.h";

using namespace std;

int main()
{
	//std::string exp = " 2 + 3 * x - 1 ";
	//std::string exp = " ( 2 + 3 ) * x - 1 ";
	std::string exp = " x / 0.5 + 4 * x ";
	//std::string exp = " 1 + 1.5 / ( x * 8 ) ";


	//std::string exp = "( 3 / x ) + ( 2 * x )";

	PostfixConvertor p;
	std::string converted_exp = p.InfixToPostfix(exp);
	ExpressionTree e(converted_exp);
	//e.TraverseInOrder(e.root);
	e.Differentiate(e.root);
	//cout << endl << "Derivative of: " << exp << endl;
	//e.TraverseInOrder(e.root);
	e.Simplify(e.root);
	//cout << endl << "Simplified: " << exp << endl;
	e.TraverseInOrder(e.root);

	return 0;
}