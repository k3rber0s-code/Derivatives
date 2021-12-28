#pragma once
#include <iostream>

enum NodeType {
	INT,
	VAR,
	OPE
};

class Node {
public:
	std::string value;
	NodeType nodeType;
	Node* left;
	Node* right;
	Node* next = NULL;

	Node(std::string, NodeType);
	Node();

	bool IsLeaf() { return (left == NULL && right == NULL); }
	Node* Duplicate(Node*);

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
	void Differentiate(Node* node);
private:
	bool IsOperator(char c) {
		return (c == '+' || c == '-' || c == '*'
			|| c == '/' || c == '^');
	};
};
