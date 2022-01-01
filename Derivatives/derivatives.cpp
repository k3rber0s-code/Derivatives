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
	newRoot->parent = root->parent;
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
			y->parent = z;

			z->right = x;
			x->parent = z;

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
		if (x->nodeType == OPE) {
			std::cout << "( ";
		}
		TraverseInOrder(x->left);

		std::cout << x->value << " ";

		TraverseInOrder(x->right);

		if (x->nodeType == OPE) {
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
			l->parent = multiplier1;
			multiplier1->right = dr;
			dr->parent = multiplier1;

			multiplier2->left = dl;
			dl->parent = multiplier2;
			multiplier2->right = r;
			r->parent = multiplier2;


			node->left = multiplier1;
			multiplier1->parent = node;
			node->right = multiplier2;
			multiplier2->parent = node;

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
			multiplier1->parent = subtractor1;
			multiplier2->parent = subtractor1;


			Node* dl = l->Duplicate(l);
			Differentiate(dl);
			Node* dr = r->Duplicate(r);
			Differentiate(dr);

			multiplier1->left = dl;
			multiplier1->right = r;
			dl->parent = multiplier1;
			r->parent = multiplier1;

			multiplier2->left = l;
			multiplier2->right = dr;
			l->parent = multiplier2;
			dr->parent = multiplier2;

			//right part
			Node* multiplier3 = new Node("*", OPE);
			multiplier3->left = r->Duplicate(r);
			multiplier3->right = r->Duplicate(r);
			multiplier3->left->parent = multiplier3;
			multiplier3->right->parent = multiplier3;


			node->left = subtractor1;
			subtractor1->parent = node;
			node->right = multiplier3;
			multiplier3->parent = node;
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
					if (node == root) {
						root = node->right;
					}
					else {
						if (node->parent->left == node) {
							node->parent->left = node->right;
						}
						else {
							node->parent->right = node->right;
						}
					}
					node = node->right;
				}
				if (node->right->nodeType == INT && std::stod(node->right->value) == 0) {
					DeleteNode(node->right);
					if (node == root) {
						root = node->left;
					}
					else {
						if (IsLeftSon(node)) {
							node->parent->left = node->left;
						}
						else {
							node->parent->right = node->left;
						}
					}
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
					if (node == root) {
						root = node->right;
					}
					else {
						if (IsLeftSon(node)) {
							node->parent->left = node->right;
						}
						else {
							node->parent->right = node->right;
						}
					}
					node = node->right; // TODO -> -x
				}
				if (node->right->nodeType == INT && std::stod(node->right->value) == 0) {
					DeleteNode(node->right);
					if (node == root) {
						root = node->left;
					}
					else {
						if (IsLeftSon(node)) {
							node->parent->left = node->left;
						}
						else {
							node->parent->right = node->left;
						}
					}
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
					if (node == root) {
						root = node->right;
					}
					else {
						if (IsLeftSon(node)) {
							node->parent->left = node->right;
						}
						else {
							node->parent->right = node->right;
						}
					}
					node = node->right;
				}
				else if (node->right->nodeType == INT && std::stod(node->right->value) == 1) {
					DeleteNode(node->right);
					if (node == root) {
						root = node->left;
					}
					else {
						if (IsLeftSon(node)) {
							node->parent->left = node->left;
						}
						else {
							node->parent->right = node->left;
						}
					}
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
					if (node == root) {
						root = node->left;
					}
					else {
						if (IsLeftSon(node)) {
							node->parent->left = node->left;
						}
						else {
							node->parent->right = node->left;
						}
					}
					node = node->left;
				}
			}
		}
	}
}
