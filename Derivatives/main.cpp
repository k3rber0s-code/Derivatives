#include <iostream>
#include <string>
#include "derivatives.h";
#include "postfix_convertor.h";

using namespace std;

int main()
{
	//std::string exp = "a11 + b12 * ( c ^ d - e ) ^ ( f + g * h ) - i";
	//std::string exp = "( 3 / x ) + ( 2 * x )";
	std::string exp = " 2 * x * x ";


	PostfixConvertor p;
	std::string converted_exp = p.InfixToPostfix(exp);
	ExpressionTree e(converted_exp);
	e.TraverseInOrder(e.root);
	e.Differentiate(e.root);
	cout << endl;
	e.TraverseInOrder(e.root);
	
	return 0;
}