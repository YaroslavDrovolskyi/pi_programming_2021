#pragma once

#ifndef BST_
#define BST_

#include <iostream>
#include <cassert>
#include <vector>

#include "Time.h"

struct BinTreeNode {
	Time data;
	BinTreeNode* left;
	BinTreeNode* right;
};


BinTreeNode* create_treenode(Time data, BinTreeNode* left = nullptr, BinTreeNode* right = nullptr) {
	BinTreeNode* new_node = new BinTreeNode;
	new_node->data = data;
	new_node->left = left;
	new_node->right = right;
}

BinTreeNode* create_bst() {
	BinTreeNode* tree = nullptr;
	return tree;
}

BinTreeNode* add(BinTreeNode* root, Time data) {
	if (root == nullptr) {
		root = create_treenode(data);
	}
	else {
		if (data < root->data) {
			root->left = add(root->left, data);
		}
		else if (data > root->data) {
			root->right = add(root->right, data);
		}
	}
	return root;
}

void print_as_tree_impl(BinTreeNode* root) {
	if (!root) { return; }
	std::cout << root->data << "(";
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

void print_as_tree(BinTreeNode* root) {
	if (!root) { std::cout << "\nTree is empty\n"; }
	else {
		print_as_tree_impl(root);
	}
}

void print_impl(BinTreeNode* root) {
	if (!root) { return; }
	print_impl(root->left);
	std::cout << root->data << std::endl;
	print_impl(root->right);
}

void print(BinTreeNode* root) {
	if (!root) { std::cout << "\nTree is empty\n"; }
	else {
		print_impl(root);
	}
}

BinTreeNode*& search_prev_child(BinTreeNode* node) {
	BinTreeNode*& current = node->left;
	if (!current) { return current; } // no previous child
	while (current->right) {
		current = current->right;
	}
	return current;
}

bool remove(BinTreeNode*& node, Time data) {
	if (node == nullptr){ return false; }
	if (data == node->data) {
		if (node->left) {
			if (node->right) { // both childs are exist
				BinTreeNode*& prev = search_prev_child(node);
				assert(prev != nullptr);
				node->data = prev->data;
				remove(prev, prev->data);
			}
			else {
				BinTreeNode* to_delete = node;
				node = node->left;
				delete to_delete;
			}
		}
		else {
			if (node->right) {
				BinTreeNode* to_delete = node;
				node = node->right;
				delete to_delete;
			}
			else { // no childs
				BinTreeNode* to_delete = node;
				node = nullptr;
				delete to_delete;
			}
		}
		return true;
	}
	else if (data < node->data) {
		return remove(node->left, data);
	}
	else {
		return remove(node->right, data);
	}

}

BinTreeNode* search(BinTreeNode* root, Time data) {
	if (root == nullptr) { return nullptr; }

	if (data == root->data) {
		return root;
	}
	else if (data < root->data) {
		return search(root->left, data);
	}
	else if (data > root->data) {
		return search(root->right, data);
	}
}

void search_impl(BinTreeNode* root, Time start, Time end, std::vector<BinTreeNode*> result) {
	if (root == nullptr) { return; }

	if (root->data >= start && root->data <= end) {
		result.push_back(root);
	}
}

template <typename Callable>
void process_list(BinTreeNode* root, Callable process) {
	if (root == nullptr) { return; }

	process_list(root->left, process);
	process(root->data);
	process_list(rocess->right, process);
}



/*



BinTreeNode* search_one(double key, double delta) { // comparison according to delta-neighborhood
	if (this->data.distance <= key + delta && this->data.distance >= key - delta) {
		return this;
	}
	else if (key + delta < this->data.distance) { // left tree
		if (this->left) {
			return this->left->search_one(key, delta);
		}
		else {
			return nullptr;
		}
	}
	else if (key - delta > this->data.distance) { // right tree
		if (this->right) {
			return this->right->search_one(key, delta);
		}
		else {
			return nullptr;
		}
	}
}

void search_all(double key, double delta, std::vector<BinTreeNode*>& result) {
	double difference = key - this->data.distance;
	if (this->data.distance <= key + delta && this->data.distance >= key - delta) { // we need to check two subtrees
		result.push_back(this);
		if (this->left) {
			this->left->search_all(key, delta, result);
		}
		if (this->right) {
			this->right->search_all(key, delta, result);
		}
	}
	else if (key + delta < this->data.distance) { // left tree
		if (this->left) {
			this->left->search_all(key, delta, result);
		}
	}
	else if (key - delta > this->data.distance) { // right tree
		if (this->right) {
			this->right->search_all(key, delta, result);
		}
	}
}

BinTreeNode*& find_previous_child() {
	BinTreeNode*& current = this->left;
	if (!current) { return current; } //no previous child
	while (current->right) {
		current = current->right;
	}
	return current;
}





struct BinSearchTree {
	BinTreeNode* root;

	BinSearchTree() {
		this->root = nullptr;
	}

	BinTreeNode* insert(Pair data) {
		if (this->root) {
			return this->root->insert(data);
		}
		else {
			this->root = new BinTreeNode(data);
			return this->root;
		}
	}

	BinTreeNode* search_one(double key, double delta) {
		if (this->root) {
			return this->root->search_one(key, delta);
		}
		else {
			return nullptr;
		}
	}

	std::vector<BinTreeNode*> search_all(double key, double delta) {
		std::vector<BinTreeNode*> result;
		if (this->root) {
			this->root->search_all(key, delta, result);
		}
		return result;
	}

	
	bool remove_one(double key, double delta) {
		if (this->root) {
			return remove_node(this->root, key, delta);
		}
		else { return false; }
	}

	bool remove_all(double key, double delta) {
		if (this->root) {
			while (this->root && remove_node(this->root, key, delta)) {
				;
			}
			return true;
		}
		else { return false; }
	}
};

BinSearchTree generate_random_BST(std::size_t size) {
	BinSearchTree tree;
	Film* films = generate_random_films(size);
	for (std::size_t i = 0; i < size - 1; i++) {
		for (std::size_t j = i + 1; j < size; j++) {
			Pair new_pair(films[i], films[j]);
			tree.insert(new_pair);
		}
	}
	delete[]films;
	return tree;
}

void remove_bin_tree(BinTreeNode* root) {
	if (root) { return; }
	if (root->left) {
		remove_bin_tree(root->left);
	}
	if (root->right) {
		remove_bin_tree(root->right);
	}
	delete root;
}

*/













#endif BST_