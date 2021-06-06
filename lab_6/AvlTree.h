#pragma once


#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <cassert>
#include <vector>

#include "Time.h"



struct AvlNode {
	Time data;
	int height;
	AvlNode* left;
	AvlNode* right;
};

AvlNode* create_avlnode(Time data, int height = 1, AvlNode* left = nullptr, AvlNode* right = nullptr) {
	AvlNode* new_node = new AvlNode;
	new_node->data = data;
	new_node->height = height;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

AvlNode* create_avl() {
	AvlNode* new_node = nullptr;
	return new_node;
}

void print_as_tree_impl(AvlNode* root) {
	if (!root) { return; }
	//	std::cout << root->data << "(";
	std::cout << root->data.year << "("; // only for testing
	if (root->left || root->right) {
		if (root->left) {
			print_as_tree_impl(root->left);
			std::cout << ", ";
		}
		else {
			std::cout << " , ";
		}
		print_as_tree_impl(root->right);

	}

	std::cout << ")";
}

void print_as_tree(AvlNode* root) {
	if (!root) { std::cout << "\nAVL tree is empty"; }
	else {
		print_as_tree_impl(root);
	}
	std::cout << std::endl;
}

void print_impl(AvlNode* root) {
	if (!root) { return; }
	print_impl(root->left);
	std::cout << root->data << "   " << root->height <<  std::endl; // only for testing
	print_impl(root->right);
}

void print(AvlNode* root) {
	if (!root) { std::cout << "\nAVL tree is empty\n"; }
	else {
		print_impl(root);
	}
}

int max(int a, int b) {
	return (a > b) ? a : b;
}

int height(AvlNode* node) {
	return node ? node->height : 0;
}

int get_balance(AvlNode* node) {
	return height(node->right) - height(node->left);
}

void fix_height(AvlNode* node) {
	node->height = max(height(node->left), height(node->right)) + 1;
}

void left_rotate(AvlNode*& node) { // right-right case
	AvlNode* b = node->right;
	node->right = b->left;
	b->left = node;
	node = b;

	fix_height(node->left);
	fix_height(node);
	
}

void right_rotate(AvlNode*& node) { // left-left case
	AvlNode* b = node->left;
	node->left = b->right;
	b->right = node;
	node = b;

	fix_height(node->right);
	fix_height(node);
	
}

void double_left_rotate(AvlNode*& node) { // right-left case
	right_rotate(node->right);
	left_rotate(node);
}

void double_right_rotate(AvlNode*& node) { // right-left case
	left_rotate(node->left); // was node->right and error
	right_rotate(node);
}

void balancing(AvlNode*& node) {
	fix_height(node);
	if (get_balance(node) == 2) {
		if (get_balance(node->right) < 0) {
			double_left_rotate(node);
		}
		else {
			left_rotate(node);
		}
	}
	else if (get_balance(node) == -2) {
		if (get_balance(node->left) > 0) {
			double_right_rotate(node);
		}
		else {
			right_rotate(node);
		}
	}
}
/*
1- right
0 - equal
-1 - left
*/

AvlNode* add(AvlNode*& root, Time data) {
	AvlNode* result = nullptr;
	if (root == nullptr) {
		root = create_avlnode(data);
		result = root;
	}
	else if (data == root->data) {
		return nullptr;
	}
	else if (data < root->data) {
		result = add(root->left, data);
	}
	else if (data > root->data) {
		result = add(root->right, data);
	}

	if (result != nullptr) {
		balancing(root);
	}
	return result;
}


AvlNode* get_min(AvlNode* root) {
	if (root == nullptr) { return nullptr; }
	AvlNode* current = root;
	while (current->left) {
		current = current->left;
	}
	return current;
}

AvlNode* remove_min(AvlNode* root, Time& min_data) {
	if (root == nullptr) { return nullptr; }
	if (root->left == nullptr) { // we are on min item now
		min_data = root->data;
		AvlNode* right = root->right;
		delete root;
		return right;
	}
	else {
		root->left = remove_min(root->left, min_data);
	}
	balancing(root);
	return root;
}
AvlNode* remove_impl(AvlNode* root, Time data) {
	if (root == nullptr) {
		return nullptr;
	}
	else if (data == root->data) {
		if (root->right) {
			Time min;
			root->right = remove_min(root->right, min);
			root->data = min;
		}
		else {
			AvlNode* left = root->left;
			delete root;
			return left;
		}
//		balancing(root);
	}
	else if (data < root->data) {
		root->left = remove_impl(root->left, data);
	}
	else if (data > root->data) {
		root->right = remove_impl(root->right, data);
	}
	balancing(root);
	return root;
}

void remove(AvlNode*& root, Time data) {
	root = remove_impl(root, data);
}











/*
AvlNode* add(AvlNode* root, Time data) {
	if (root == nullptr) {
		root = create_avlnode(data);
		return root;
	}
	else if (data == root->data) {
		return nullptr; // don't need to duplicate
	}
	else if (data < root->data) {
		if (root->left) {
			int prev_balance = root->left->balance;
			AvlNode* result = add(root->left, data);
			int cur_balance = root->left->balance;
			if (!result) { return nullptr; } // we didn't add anything
			if (prev_balance != cur_balance) {
				root->balance--;
				if (root->balance < -1) {
					if (cur_balance < prev_balance) {
						// left-left rotation of root // right rotation
					}
					else if (cur_balance > prev_balance) {
						// left-right rotation of root
					}
				}
			}
			/*
			if (prev_balance == 0) {
				if (cur_balance != 0) {
					root->balance--;
					if (root->balance < -1) {
						if (cur_balance == -1) {
							// left-left rotation of root
						}
						else if (cur_balance == 1) {
							// left-right rotation of root
						}
					}
				}
			}

			return result;
		}
		else {
			root->left = create_avlnode(data);
			root->balance--;
			return root->left;
		}
	}
	else if (data > root->data) {
		if (root->right) {
			int prev_balance = root->right->balance;
			AvlNode* result = add(root->right, data);
			int cur_balance = root->right->balance;
			if (!result) { return nullptr; } // we didn't add anything
			if (prev_balance != cur_balance) {
				root->balance++;
				if (root->balance > 1) {
					if (cur_balance < prev_balance) {
						// right-left rotation of root
					}
					else if (cur_balance > prev_balance) {
						// right-right rotation of root
					}
				}
			}
			/*
			if (prev_balance == 0) {
				if (cur_balance != 0) {
					root->balance--;
					if (root->balance < -1) {
						if (cur_balance == -1) {
							// left-left rotation of root
						}
						else if (cur_balance == 1) {
							// left-right rotation of root
						}
					}
				}
			}

			return result;
		}
		else {
			root->left = create_avlnode(data);
			root->balance--;
			return root->left;
		}
	}
}
*/










#endif // AVLTREE_H