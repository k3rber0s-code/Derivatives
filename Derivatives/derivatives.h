#pragma once
#include <iostream>
class Node {
public:
	std::string value;
	Node* left;
	Node* right;
	Node* next = NULL;

	Node(std::string);
	Node();

	friend class ExpressionTree;
	friend class Stack;
};
class Stack {
	Node* head = NULL;
public:
	void push(Node*);
	Node* pop();

	friend class ExpressionTree;
};
class ExpressionTree {
public:
	ExpressionTree(std::string expression);
	~ExpressionTree() {};

	Node* root;
	void TraverseInOrder(Node* x);
private:
	bool IsOperator(char c) {
		return (c == '+' || c == '-' || c == '*'
			|| c == '/' || c == '^');
	};
};