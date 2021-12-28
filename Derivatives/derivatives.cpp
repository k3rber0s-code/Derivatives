#include "derivatives.h"
Node::Node(std::string v, NodeType n) {
	nodeType = n;
	value = v;
	left = NULL;
	right = NULL;
}
Node::Node() {
	left = NULL;
	right = NULL;
}
Node* Node::Duplicate(Node* root)
{
	Node* newRoot = new Node(root->value, root->nodeType);
	if (root->left != NULL) {
		newRoot->left = Duplicate(root->left);
	}
	else if (root->right != NULL) {
		newRoot->left = Duplicate(root->right);
	}
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
	root = NULL;
	Stack e;
	Node* x, * y, * z;
	int l = expression.length();
	std::string temp = "";
	for (int i = 0; i < l; i++) {
		if (expression[i] == ' ') {
			if (temp != "") {
				if (isalpha(temp[0])) {
					z = new Node(temp, VAR);
				}
				else {
					z = new Node(temp, INT);
				}
				e.push(z);
				temp = "";
			}
			continue;
		}
		if (IsOperator(expression[i])) {
			z = new Node(std::string(1, expression[i]), OPE);
			x = e.pop();
			y = e.pop();
			z->left = y;
			z->right = x;
			e.push(z);
		}
		else {
			temp += expression[i];
		}
		root = std::move(z);
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

void ExpressionTree::Differentiate(Node* node)
{
	// empty tree
	if (node == NULL) return;

	if (node->IsLeaf()) {
		if (node->nodeType == INT) {
			node->value == "0";
		}
		else if (node->nodeType == VAR) { //nodeType VAR
			node->nodeType == INT;
			node->value == "1";
		}
		else {
			throw;
		}

	}
	else if (!node->IsLeaf()) {
		if (node->value == "+") {
			Differentiate(node->left);
			Differentiate(node->right);
		}
		else if (node->value == "-") {
			Differentiate(node->left);
			Differentiate(node->right);
		}
		else if (node->value == "*") {
			node->value == "+";
			Node* l = node->left;
			Node* r = node->right;

			Node* multiplier1 = new Node("*", OPE);
			Node* multiplier2 = new Node("*", OPE);

			node->left = multiplier1;
			node->right = multiplier2;



		}
	}
}
