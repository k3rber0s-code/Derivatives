#include <iostream>;
#include <stack>;
#include "postfix_convertor.h";
using namespace std;

//Function to return precedence of operators
int PostfixConvertor::GetPrecedence(string c) {
	if (c == "^")
		return 3;
	else if (c == "/" || c == "*")
		return 2;
	else if (c == "+" || c == "-")
		return 1;
	else
		return -1;
}

// The main function to convert infix expression
//to postfix expression
std::string PostfixConvertor::InfixToPostfix(std::string s) {

	stack<std::string> st; 
	string result;
	string temp = "";
	for (int i = 0; i < s.length(); i++) {
		char c = s[i];

		if (c == ' ') {
			if (temp != "") {
				result += temp;
				result += " ";
				temp = "";
			}
			continue;
		}
		// If the scanned character is
		// an operand, add it to output string.
		else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c=='.') {
			temp += c;
			if (i == s.length() - 1) {
				result += temp;
				result += " ";
			}
		}

		// If the scanned character is an
		// ‘(‘, push it to the stack.
		else if (c == '(')
			st.push(string(1, '('));

		// If the scanned character is an ‘)’,
		// pop and to output string from the stack
		// until an ‘(‘ is encountered.
		else if (c == ')') {
			while (st.top() != "(")
			{
				result += st.top();
				result += " ";
				st.pop();
				if (st.size() == 0) {
					std:: cout << "Invalid input" << s << std::endl;
					return "";
				}
			}
			st.pop();
		}

		//If an operator is scanned
		else {
			while (!st.empty() && GetPrecedence(string(1, s[i])) <= GetPrecedence(st.top())) {
				result += st.top();
				result += " ";
				st.pop();
			}
			st.push(string(1, c));
		}
	}

	// Pop all the remaining elements from the stack
	while (!st.empty()) {
		result += st.top();
		result += " ";
		st.pop();
	}
	//cout << result << endl;
	return result;
}

