#include <iostream>
#include <string>
#include <algorithm>


using namespace std;

struct Node
{
	int data; // hold the key
	Node* parent; 
	Node* left; 
	Node* right; 
	int color; // 0 - Black |  1 - Red 
};

typedef Node* NodePtr;

class Tree 
{
private:
	NodePtr root;
	NodePtr TNULL;

	void initializeNULLNode(NodePtr node, NodePtr parent) 
	{
		node->data = 0;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = 0;
	}

	void preorderHelp(NodePtr node) 
	{
		if (node != TNULL) 
		{
			cout << node->data << " ";
			preorderHelp(node->left);
			preorderHelp(node->right);
		}
	}

	void inorderHelp(NodePtr node) 
	{
		if (node != TNULL)
		{
			inorderHelp(node->left);
			cout << node->data << " ";
			inorderHelp(node->right);
		}
	}

	void postorderHelp(NodePtr node) 
	{
		if (node != TNULL)
		{
			postorderHelp(node->left);
			postorderHelp(node->right);
			cout << node->data << " ";
		}
	}

	NodePtr searchTreeHelp(NodePtr node, int key) 
	{
		if (node == TNULL || key == node->data) 
		{
			return node;
		}

		if (key < node->data) 
		{
			return searchTreeHelp(node->left, key);
		}
		return searchTreeHelp(node->right, key);
	}

	// fix the tree by delete operation
	void fixDelete(NodePtr x) 
	{
		NodePtr s;

		while (x != root && x->color == 0) 
		{
			if (x == x->parent->left) 
			{
				s = x->parent->right;
				if (s->color == 1) 
				{
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) 
				{
					s->color = 1;
					x = x->parent;
				}
				else 
				{
					if (s->right->color == 0) 
					{
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					}

					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			}

			else 
			{
				s = x->parent->left;
				if (s->color == 1) 
				{
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == 0 && s->right->color == 0) 
				{
					s->color = 1;
					x = x->parent;
				}
				else 
				{
					if (s->left->color == 0) 
					{
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					}

					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = 0;
	}


	void rbTransplant(NodePtr u, NodePtr v) 
	{
		if (u->parent == nullptr) 
		{
			root = v;
		}
		else if (u == u->parent->left) 
		{
			u->parent->left = v;
		}
		else 
		{
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNodeHelper(NodePtr node, int key) 
	{
		// find node key
		NodePtr z = TNULL;
		NodePtr x, y;

		while (node != TNULL) 
		{
			if (node->data == key) 
			{
				z = node;
			}

			if (node->data <= key) 
			{
				node = node->right;
			}
			else {
				node = node->left;
			}
		}

		if (z == TNULL) 
		{
			cout << "Couldn't find key in the tree" << endl;
			return;
		}

		y = z;
		int y_original_color = y->color;

		if (z->left == TNULL) 
		{
			x = z->right;
			rbTransplant(z, z->right);
		}
		else if (z->right == TNULL) 
		{
			x = z->left;
			rbTransplant(z, z->left);
		}
		else 
		{
			y = min(z->right);
			y_original_color = y->color;
			x = y->right;

			if (y->parent == z) 
			{
				x->parent = y;
			}
			else 
			{
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;

		if (y_original_color == 0) 
		{
			fixDelete(x);
		}
	}

	// fix the tree
	void fixInsert(NodePtr k) 
	{
		NodePtr uncle;

		while (k->parent->color == 1) 
		{
			if (k->parent == k->parent->parent->right) 
			{
				uncle = k->parent->parent->left; 
				if (uncle->color == 1) {
					uncle->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else 
				{
					if (k == k->parent->left) 
					{
						k = k->parent;
						rightRotate(k);
					}
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			}
			else 
			{
				uncle = k->parent->parent->right; 

				if (uncle->color == 1)
				{
					uncle->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else 
				{
					if (k == k->parent->right) 
					{
						k = k->parent;
						leftRotate(k);
					}
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) 
			{
				break;
			}
		}
		root->color = 0;
	}

	void printHelp(NodePtr root, string indent, bool last) 
	{
		// print the tree structure on the screen
		if (root != TNULL) 
		{
			cout << indent;
			if (last) 
			{
				cout << "R----";
				indent += "     ";
			}
			else 
			{
				cout << "L----";
				indent += "|    ";
			}

			string sColor = root->color ? "RED" : "BLACK";
			cout << root->data << "(" << sColor << ")" << endl;
			printHelp(root->left, indent, false);
			printHelp(root->right, indent, true);
		}
	}

public:
	Tree() 
	{
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}

	void preorder() 
	{
		preorderHelp(this->root);
	}

	void inorder() 
	{
		inorderHelp(this->root);
	}

	void postorder() 
	{
		postorderHelp(this->root);
	}

	NodePtr searchTree(int k) 
	{
		return searchTreeHelp(this->root, k);
	}

	NodePtr min(NodePtr node) 
	{
		while (node->left != TNULL) 
		{
			node = node->left;
		}
		return node;
	}

	NodePtr max(NodePtr node) 
	{
		while (node->right != TNULL) 
		{
			node = node->right;
		}
		return node;
	}

	NodePtr successor(NodePtr x) 
	{

		if (x->right != TNULL) 
		{
			return min(x->right);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->right) 
		{
			x = y;
			y = y->parent;
		}
		return y;
	}

	NodePtr predecessor(NodePtr x) 
	{
		if (x->left != TNULL) 
		{
			return max(x->left);
		}

		NodePtr y = x->parent;
		while (y != TNULL && x == y->left) 
		{
			x = y;
			y = y->parent;
		}

		return y;
	}

	void leftRotate(NodePtr x) 
	{
		NodePtr y = x->right;
		x->right = y->left;

		if (y->left != TNULL) 
		{
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) 
		{
			this->root = y;
		}
		else if (x == x->parent->left) 
		{
			x->parent->left = y;
		}
		else 
		{
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	void rightRotate(NodePtr x) 
	{
		NodePtr y = x->left;
		x->left = y->right;

		if (y->right != TNULL) 
		{
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) 
		{
			this->root = y;
		}
		else if (x == x->parent->right) 
		{
			x->parent->right = y;
		}
		else 
		{
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	void insert(int key)
	{
		NodePtr node = new Node;
		node->parent = nullptr;
		node->data = key;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; 

		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL) 
		{
			y = x;
			if (node->data < x->data) 
			{
				x = x->left;
			}
			else 
			{
				x = x->right;
			}
		}

		node->parent = y;

		if (y == nullptr) 
		{
			root = node;
		}
		else if (node->data < y->data) 
		{
			y->left = node;
		}
		else 
		{
			y->right = node;
		}

		if (node->parent == nullptr) 
		{
			node->color = 0;
			return;
		}

		if (node->parent->parent == nullptr) 
		{
			return;
		}

		fixInsert(node);
	}

	NodePtr getRoot() 
	{
		return this->root;
	}

	void deleteNode(int data) 
	{
		deleteNodeHelper(this->root, data);
	}

	void Print() 
	{
		if (root) 
		{
			printHelp(this->root, "", true);
		}
	}

};

int main() {
	Tree test;
	test.insert(13);
	test.insert(8);
	test.insert(17);
	test.insert(1);
	test.insert(11);
	test.insert(15);
	test.insert(25);
	test.insert(6);
	test.insert(22);
	test.insert(27);
	test.insert(45);
	test.deleteNode(45);
	test.Print();
	return 0;
}