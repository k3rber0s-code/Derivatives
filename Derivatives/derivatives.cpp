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
void Stack::Push(Node* x)
{
	if (head == NULL) {
		head = x;
	}
	else {
		x->next = head;
		head = x;
	}
}

Node* Stack::Pop()
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
				e.Push(z);
				temp = "";
			}
			continue;
		}
		if (IsOperator(expression[i])) {
			z = new Node(std::string(1, expression[i]), OPE);
			if (e.head == NULL || e.head->next == NULL) {
				std::cout << "Invalid input" << std::endl;
				return;
			}
			x = e.Pop();
			y = e.Pop();
			z->left = y;
			z->right = x;
			e.Push(z);
		}
		else {
			temp += expression[i];
		}
		root = std::move(z);
	}
}

void ExpressionTree::DeleteNode(Node* node)
{
	if (node == root) {
		root = NULL;
	}
	if (node == NULL) {
		return;
	}
	DeleteNode(node->left);
	DeleteNode(node->right);
	delete node;
}

void ExpressionTree::TraverseInOrder(Node* x)
{
	if (x == NULL) {
		return;
	}
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
			node->nodeType = INT;
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

void ExpressionTree::Simplify(Node* node)
{
	if (!node->left->IsLeaf()) {
		Simplify(node->left);
	}
	if (!node->right->IsLeaf()) {
		Simplify(node->right);
	}

	if (node->nodeType == OPE) {
		if (node->value == "+") {
			if (node->left->IsLeaf() && node->right->IsLeaf()
				&& node->left->nodeType == INT && node->right->nodeType == INT) {
				double l = std::stod(node->left->value);
				double r = std::stod(node->right->value);
				std::string result = std::to_string(l + r);

				result.erase(result.find_last_not_of('0') + 1, std::string::npos);
				result.erase(result.find_last_not_of('.') + 1, std::string::npos);

				node->value = result;
				node->nodeType = INT;

				DeleteNode(node->left);
				DeleteNode(node->right);

				node->left = NULL;
				node->right = NULL;
			}
			else {
				if (node->left->nodeType == INT && std::stod(node->left->value) == 0) {
					DeleteNode(node->left);
					node = node->right;
				}
				if (node->right->nodeType == INT && std::stod(node->right->value) == 0) {
					DeleteNode(node->right);
					node = node->left;
				}
			}
		}
		else if (node->value == "-") {
			if (node->left->IsLeaf() && node->right->IsLeaf()
				&& node->left->nodeType == INT && node->right->nodeType == INT) {
				double l = std::stod(node->left->value);
				double r = std::stod(node->right->value);
				std::string result = std::to_string(l - r);

				result.erase(result.find_last_not_of('0') + 1, std::string::npos);
				result.erase(result.find_last_not_of('.') + 1, std::string::npos);

				node->value = result;
				node->nodeType = INT;

				DeleteNode(node->left);
				DeleteNode(node->right);

				node->left = nullptr;
				node->right = NULL;
			}
			else {
				if (node->left->nodeType == INT && std::stod(node->left->value) == 0) {
					DeleteNode(node->left);
					node = node->right;
				}
				if (node->right->nodeType == INT && std::stod(node->right->value) == 0) {
					DeleteNode(node->right);
					node = node->left;
				}
			}
		}
		else if (node->value == "*") {
			if (node->left->IsLeaf() && node->right->IsLeaf()
				&& node->left->nodeType == INT && node->right->nodeType == INT) {
				double l = std::stod(node->left->value);
				double r = std::stod(node->right->value);
				std::string result = std::to_string(l * r);

				result.erase(result.find_last_not_of('0') + 1, std::string::npos); 
				result.erase(result.find_last_not_of('.') + 1, std::string::npos);

				node->value = result;
				node->nodeType = INT;

				DeleteNode(node->left);
				DeleteNode(node->right);

				node->left = nullptr;
				node->right = NULL;
			}
			else {
				if (node->left->nodeType == INT && std::stod(node->left->value) == 0) {
					DeleteNode(node->left);
					DeleteNode(node->right);
					node->value = "0";
					node->nodeType = INT;
					node->left = NULL;
					node->right = NULL;
				}
				else if (node->right->nodeType == INT && std::stod(node->right->value) == 0) {
					DeleteNode(node->left);
					DeleteNode(node->right);
					node->value = "0";
					node->nodeType = INT;
					node->left = NULL;
					node->right = NULL;
				}
				else if (node->left->nodeType == INT && std::stod(node->left->value) == 1) {
					DeleteNode(node->left);
					node = node->right;
				}
				else if (node->right->nodeType == INT && std::stod(node->right->value) == 1) {
					DeleteNode(node->right);
					node = node->left;
				}
			}
		}
		else if (node->value == "/") {
			if (node->left->IsLeaf() && node->right->IsLeaf()
				&& node->left->nodeType == INT && node->right->nodeType == INT) {
				double l = std::stod(node->left->value);
				double r = std::stod(node->right->value);
				std::string result = std::to_string(l / r);

				result.erase(result.find_last_not_of('0') + 1, std::string::npos);
				result.erase(result.find_last_not_of('.') + 1, std::string::npos);

				node->value = result;
				node->nodeType = INT;

				DeleteNode(node->left);
				DeleteNode(node->right);

				node->left = NULL;
				node->right = NULL;
			}
			else {
				if (node->left->nodeType == INT && std::stod(node->left->value) == 0) {
					DeleteNode(node->left);
					DeleteNode(node->right);
					node->value = "0";
					node->nodeType = INT;
					node->left = NULL;
					node->right = NULL;
				}
				else if (node->right->nodeType == INT && std::stod(node->right->value) == 1) {
					DeleteNode(node->right);
					node = node->left;
				}
			}
		}
	}
}
