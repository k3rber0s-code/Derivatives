#include "derivatives.h"
Node::Node(std::string v) {
	value = v;
	left = NULL;
	right = NULL;
}
Node::Node() {
	left = NULL;
	right = NULL;
}
void Stack::push(Node* x)
{
	if (head == NULL) {
		head = x;
	}
	else {
		x->next = head;
		head = x;
	}
}

Node* Stack::pop()
{
	Node* p = head;
	head = head->next;
	return p;
}

ExpressionTree::ExpressionTree(std::string expression)
{
	Stack e;
	Node* x, * y, * z;
	int l = expression.length();
	std::string temp = "";
	for (int i = 0; i < l; i++) {
		if (expression[i] == ' ') {
			if (temp != "") {
				z = new Node(temp);
				e.push(z);
				temp = "";
			}
			continue;
		}
		if (IsOperator(expression[i])) {
			z = new Node(std::string(1, expression[i]));
			x = e.pop();
			y = e.pop();
			z->left = y;
			z->right = x;
			e.push(z);
		}
		else {
			temp += expression[i];
		}
	}
}

void ExpressionTree::TraverseInOrder(Node* x)
{
	if (x == NULL)
		return;
	else {
		TraverseInOrder(x->left);
		std::cout << x->value << "  ";
		TraverseInOrder(x->right);
	}
}
