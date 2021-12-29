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
	if (root->right != NULL) {
		newRoot->right = Duplicate(root->right);
	}
	return newRoot;
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
		if (x->nodeType == OPE && x != root) {
			std::cout << "( ";
		}
		TraverseInOrder(x->left);

		std::cout << x->value << " ";

		TraverseInOrder(x->right);

		if (x->nodeType == OPE && x != root) {
			std::cout << ") ";
		}



	}
}

void ExpressionTree::Differentiate(Node* node)
{
	// empty tree
	if (node == NULL) return;

	if (node->IsLeaf()) {
		if (node->nodeType == INT) {
			node->value = "0";
		}
		else if (node->nodeType == VAR) { 
			node->nodeType == INT;
			node->value = "1";
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
			node->value = "+";
			Node* l = node->left;
			Node* r = node->right;

			Node* multiplier1 = new Node("*", OPE);
			Node* multiplier2 = new Node("*", OPE);

			Node* dl = l->Duplicate(l);
			Differentiate(dl);
			Node* dr = r->Duplicate(r);
			Differentiate(dr);

			multiplier1->left = l;
			multiplier1->right = dr;

			multiplier2->left = dl;
			multiplier2->right = r;


			node->left = multiplier1;
			node->right = multiplier2;
		}
		else if (node->value == "/") {
			Node* l = node->left;
			Node* r = node->right;

			//left part

			Node* subtractor1 = new Node("-", OPE);

			Node* multiplier1 = new Node("*", OPE);
			Node* multiplier2 = new Node("*", OPE);

			subtractor1->left = multiplier1;
			subtractor1->right = multiplier2;

			Node* dl = l->Duplicate(l);
			Differentiate(dl);
			Node* dr = r->Duplicate(r);
			Differentiate(dr);

			multiplier1->left = l;
			multiplier1->right = dr;

			multiplier2->left = dl;
			multiplier2->right = r;

			//right part
			Node* multiplier3 = new Node("*", OPE);
			multiplier3->left = r->Duplicate(r);
			multiplier3->right = r->Duplicate(r);

			node->left = subtractor1;
			node->right = multiplier3;
		}
	}
}
