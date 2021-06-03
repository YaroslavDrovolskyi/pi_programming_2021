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












#endif // AVLTREE_H