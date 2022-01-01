#pragma once
#include <iostream>
#include <string>

enum NodeType {
	INT,
	VAR,
	OPE
};

class Node {
public:
	std::string value;
	NodeType nodeType;
	Node* parent;
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
	void Push(Node*);
	Node* Pop();

	friend class ExpressionTree;
};
class ExpressionTree {
public:
	ExpressionTree(std::string expression);
	~ExpressionTree() {};

	Node* root;
	void DeleteNode(Node* node);
	void TraverseInOrder(Node* node);
	void Differentiate(Node* node);
	void Simplify(Node* node);
private:
	bool IsLeftSon(Node* node) { return (node->parent->left == node); };
	bool IsOperator(char c) {
		return (c == '+' || c == '-' || c == '*'
			|| c == '/' || c == '^');
	};
};
