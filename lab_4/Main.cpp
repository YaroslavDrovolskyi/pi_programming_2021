#include <iostream>
#include <vector>
#include <cassert>




struct TreeNode {
	int data;
	TreeNode* parent;
	TreeNode* first_child;
	TreeNode* next;
	TreeNode* prev;

	TreeNode(int data, TreeNode* parent = nullptr, TreeNode* first_child = nullptr, TreeNode* next = nullptr, TreeNode* prev = nullptr) {
		this->data = data;
		this->parent = parent;
		this->first_child = first_child;
		this->next = next;
		this->prev = prev;
	}


	void print() {
		std::cout << this->data << "(";
		TreeNode* current = this->first_child;
		while (current) {
			current->print();
			if (current->next) { std::cout << ", "; }
			current = current->next;
			
		}
		std::cout << ")";
	}

	void print_with_childs() {
		std::cout << "value = " << this->data << ", parent = ";
		if (this->parent == nullptr) {
			std::cout << "NULL";
		}
		else {
			std::cout << this->parent->data;
		}
		std::cout << ", children: ";

		if (this->first_child == nullptr) {
			std::cout << "NULL;\n";
		}
		else {
			TreeNode* current = this->first_child;
			while (current) {
				if (current->next == nullptr) {
					std::cout << current->data << ";\n";
				}
				else {
					std::cout << current->data << ", ";
				}
				current = current->next;
			}
		}

		std::cout << "\nInteractive print ended\n";
	}
};

struct Tree {
	TreeNode* root;

	Tree() {
		this->root = nullptr;
	}


	TreeNode* get_by_path(std::vector<std::size_t>& path) {
		assert(this->root != nullptr);
		if (path.size() == 0) { return this->root; }
		TreeNode* current = this->root;
		for (std::size_t i = 0; i < path.size(); i++) {
			assert(current != nullptr);
			current = current->first_child;
			for (std::size_t j = 0; j < path[i]; j++) {
				assert(current != nullptr);
				current = current->next;
			}
		}
		return current;
	}

	void print() {
		if (this->root == nullptr) {
			std::cout << "\nTree is empty\n";
		}
		else {
			this->root->print();
		}
		std::cout << std::endl;
	}

	void add_by_path(int data, std::vector<std::size_t>& path) {
		if (path.size() == 0) {
			add_by_pointer(data, nullptr);
		}
		else {
			add_by_pointer(data, get_by_path(path));
		}

	}



	//private:

	void add_by_pointer(int data, TreeNode* parent = nullptr) {
		TreeNode* new_node = new TreeNode(data, parent);
		if (parent == nullptr && this->root == nullptr) {
			this->root = new_node;
		}
		else {
			TreeNode* current = parent->first_child;

			if (current) {
				while (current->next) {
					current = current->next;
				}
				current->next = new_node;
				new_node->prev = current;
			}
			else {
				parent->first_child = new_node;
			}

		}
	}

	std::vector<std::size_t> get_path(TreeNode* node) {
		assert(node != nullptr);
		std::vector<std::size_t> path;
	//	if (node == this->root) { return path; }
		//else {
			
			TreeNode* current = node;
			while (current->parent) {
				std::size_t index = 0;
				while (current->prev) {
					index++;
					current = current->prev;
				}
				path.push_back(index);
				current = current->parent;
			}
			return path;
			
		//}
	}

	void interactive_print() {
		std::cout << "\nInteractive print: \n";
		if (this->root == nullptr) {
			std::cout << "Tree is empty\n";
			return;
		}
		short int next = -1;
		//std::cout << this->root;
		TreeNode* current = this->root;
		while (next > -2) {
			current->print_with_childs();
			std::cout << "\nWhat is the next: (-2 - exit, -1 - to parent, >= 0 - index of child)\n";
			std::cin >> next;
			if (next == -1) {
				if (current->parent == nullptr) {
					std::cout << "Parent is NULL\n";
				}
				else {
					current = current->parent;
				}
			}
			else if (next > -1) {
				if (current->first_child == nullptr) {
					std::cout << "No chidren\n";
				}
				else {
					bool flag = false;
					TreeNode* current_child = current->first_child;
					for (std::size_t i = 0; i < next; i++) {
						current_child = current_child->next;
						if (current_child->next == nullptr && i < next - 1) {
							std::cout << "No child with this index\n";
							flag = true;
							break;
						}
					}
					if (flag == false) {
						current = current_child;
					}
				}
				
			}
		}
	}
};




void print_path(std::vector<std::size_t> path) {
	if (path.size() == 0) {
		std::cout << "Path is empty\n";
	}
	else {
		for (std::size_t i = 0; i < path.size(); i++) {
			std::cout << path[i] << " ";
		}
	}
	std::cout << std::endl;
}
















int main() {
	Tree tree;
	tree.add_by_pointer(5);
	tree.add_by_pointer(6, tree.root);
	//tree.add_by_pointer(7, tree.root->first_child);
	tree.add_by_pointer(45, tree.root);

	tree.print();

	std::vector<std::size_t> path;
	path.push_back(0);
	tree.add_by_path(100, path);

	//std::cout << "\n\n\n" << tree.get_by_path(path)->data << std::endl;
	tree.print();

	std::cout << std::endl;
//	print_path(tree.get_path(tree.root->first_child->first_child->parent->parent->first_child->next));
	tree.interactive_print();

	std::system("pause");
	return 0;
}