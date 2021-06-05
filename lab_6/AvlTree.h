#pragma once


#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <cassert>
#include <vector>

#include "Time.h"



struct AvlNode {
	Time data;
	int balance;
	AvlNode* left;
	AvlNode* right;
};

AvlNode* create_avlnode(Time data, int balance = 0, AvlNode* left = nullptr, AvlNode* right = nullptr) {
	AvlNode* new_node = new AvlNode;
	new_node->data = data;
	new_node->balance = balance;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

AvlNode* create_alv() {
	AvlNode* new_node = nullptr;
	return new_node;
}

void left_rotate(AvlNode*& node) {
	AvlNode* b = node->right;
	node->right = b->left;
	b->left = node;
	node = b;
}

void right_rotate(AvlNode*& node) {
	AvlNode* b = node->left;
	node->left = b->right;
	b->right = node;
	node = b;
}

/*
1- right
0 - equal
-1 - left
*/

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
					if (cur_balance == -1) {
						// left-left rotation of root
					}
					else if (cur_balance == 1) {
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
			*/
		}
		else {
			root->left = create_avlnode(data);
			root->balance--;
			return root->left;
		}
		
	}
	else if (data > root->data) {
		return add(root->right, data);
	}
}












#endif // AVLTREE_H