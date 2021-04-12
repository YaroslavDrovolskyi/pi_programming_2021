// 1, 2, 3, 7, 9, 11, 12, 13

#include <iostream>
#include <vector>
#include <cassert>



void print_path(std::vector<std::size_t> path, std::size_t max_length);
std::size_t get_number_of_digits(int value);

struct Tree;
struct TreeNode;


struct ListNode {
	ListNode* next;
	TreeNode* data;

	ListNode(TreeNode* data, ListNode* next = nullptr) {
		this->data = data;
		this->next = next;
	}
};

struct Queue {
	ListNode* begin;
	ListNode* end;
	std::size_t size;

	Queue() {
		this->begin = this->end = nullptr;
		this->size = 0;
	}

	void enqueue(TreeNode* data) {
		ListNode* new_node = new ListNode(data);
		if (this->end) {
			this->end->next = new_node;
		}
		else { // if queue is empty
			this->begin = new_node;
		}
		this->end = new_node;

		this->size++;
	}

	TreeNode* dequeue() {
		assert(this->begin != nullptr);

		TreeNode* result = this->begin->data;
		ListNode* current = this->begin;

		this->begin = this->begin->next;
		if (!this->begin) {
			this->end = this->begin;
		}
		delete current;

		return result;
	}

	bool is_empty() {
		return this->begin == nullptr;
	}

};


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


	void get_max_childs(std::size_t &max_childs_number) {
		std::size_t childs_number = 0;
		if (this->first_child != nullptr) {
			TreeNode* current = this->first_child;
			while (current) {
				childs_number++;
				current->get_max_childs(max_childs_number);
				current = current->next;
			}
		}
		if (childs_number > max_childs_number) {
			max_childs_number = childs_number;
		}
	}

	std::vector<std::size_t> get_path() {
		std::vector<std::size_t> path;
		//	if (node == this->root) { return path; }
			//else {

		TreeNode* current = this;
		while (current->parent) {
			std::size_t index = 0;
			while (current->prev) {
				index++;
				current = current->prev;
			}
			path.push_back(index);
			current = current->parent;
		}

		std::reverse(path.begin(), path.end());
		return path;

		//}
	}

	void print_with_path(std::size_t max_length) {
		print_path(this->get_path(), max_length);
		std::cout << this->data << std::endl;
		TreeNode* current = this->first_child;
		while(current) {
			current->print_with_path(max_length);
			current = current->next;
		}
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
			std::cout << "\nTree with brackets:\n";
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
		return node->get_path();
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


	void print_with_path() {
		if (this->root == nullptr) {
			std::cout << "\nTree is empty\n";
		}
		else {
			std::cout << "\nTree with paths\n";
			std::size_t max_childs = 0;
			this->root->get_max_childs(max_childs);
			std::size_t max_length = get_number_of_digits(max_childs);
			this->root->print_with_path(max_length);
		}
		
	}

	Tree remove_by_path(std::vector<std::size_t>& path) {
		assert(this->root != nullptr);
		TreeNode* node_to_remove = get_by_path(path);
		
		return remove_in_tree(node_to_remove);
	}



	Tree remove_in_tree(TreeNode* node_to_remove) {
		Tree new_tree;
		new_tree.root = node_to_remove;

		if (node_to_remove->parent) {
			if (node_to_remove->next) {
				node_to_remove->next->prev = node_to_remove->prev;
			}
			if (node_to_remove->prev) {
				node_to_remove->prev->next = node_to_remove->next;
			}
			else {
				node_to_remove->parent->first_child = node_to_remove->next;
			}
		}
		return new_tree;
	}


	void print_levels() {
		if (this->root == nullptr) {
			std::cout << "\nTree is empty\n";
		}
		else {
			std::cout << "\nTree by levels:\n";
			TreeNode* const DELIMITER = nullptr;
			Queue queue;
			queue.enqueue(this->root);
			queue.enqueue(DELIMITER);
			while (true) {
				TreeNode* current = queue.dequeue();
				if (current != DELIMITER) {
					std::cout << current->data << " ";

					TreeNode* current_child = current->first_child;
					while (current_child) {
						queue.enqueue(current_child);

						current_child = current_child->next;
					}

				}
				else {
					std::cout << std::endl;
					if (queue.is_empty() == true) {
						break;
					}
					else {
						queue.enqueue(DELIMITER);
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

void print_path(std::vector<std::size_t> path, std::size_t max_length) {
	if (path.size() == 0) {
		for (std::size_t i = 0; i < max_length; i++) { // gap that = maximal length
			std::cout << " ";
		}
	}
	else {
		for (std::size_t i = 0; i < path.size(); i++) {
			std::cout << path[i];
			std::size_t current_length = get_number_of_digits(path[i]);
			// print missing symbols
			for (std::size_t j = 0; j < max_length - current_length; j++) {
				std::cout << " ";
			}
			
			std::cout << " "; // obvious gap
		}
	}
	std::cout << ": "; // gap between path and data
}


std::size_t max_in_vector(std::vector <std::size_t> array) {
	assert(array.size() > 0);
	std::size_t min_index = 0;
	for (std::size_t i = 1; i < array.size(); i++) {
		if (array[i] < array[min_index]) {
			min_index = i;
		}
	}
	return array[min_index];
}


std::size_t get_number_of_digits(int value) {
	if (value == 0) { return 1; }
	std::size_t i = 0;
	while (value > 0) {
		i++;
		value /= 10;
	}
	return i;
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
	tree.add_by_path(100, path); // 0
	tree.add_by_path(92, path);
	tree.add_by_path(90, path);
	tree.add_by_path(98, path);

	path.push_back(1); // 0, 1
	tree.add_by_path(73, path);
	tree.add_by_path(75, path);

	path.push_back(1); // 0, 1, 1
	tree.add_by_path(31, path);

	path.clear();
	path.push_back(0);
	path.push_back(3); // 0, 3
	tree.add_by_path(95, path);

	path.push_back(0); // 0, 3, 0
	tree.add_by_path(66, path);
	tree.add_by_path(67, path);
	tree.add_by_path(68, path);

	path.clear();
	path.push_back(1); // 1 
	tree.add_by_path(56, path);
	tree.add_by_path(79, path);

	path.push_back(0); // 1, 0
	tree.add_by_path(71, path);
	tree.add_by_path(150, path);

	path.push_back(1); // 1, 0, 1
	tree.add_by_path(53, path);
	tree.add_by_path(61, path);
	tree.add_by_path(65, path);

	path.clear();
	path.push_back(1);
	path.push_back(1); // 1, 1 
	tree.add_by_path(33, path);

	//std::cout << "\n\n\n" << tree.get_by_path(path)->data << std::endl;
	tree.print();

	std::cout << std::endl;
	print_path(tree.get_path(tree.root->first_child->first_child->parent->parent->first_child->next));
//	tree.interactive_print();

	std::size_t max_childs = 0;
	tree.root->get_max_childs(max_childs);
	std::cout << std::endl << "max childs = " << max_childs << std::endl;
	std::cout << std::endl << "number of digits of 1535 = " << get_number_of_digits(1535) << std::endl;
	std::cout << std::endl << "print with path:" << std::endl;
	tree.print_with_path();



//	Tree new_tree = tree.remove_by_path(path);
//	new_tree.print();
//	tree.print();
	tree.print_levels();


	path.clear();
	path.push_back(0);
	path.push_back(3); // 0, 3

	Tree new_tree = tree.remove_by_path(path);
	new_tree.print();
	tree.print_levels();
	std::system("pause");
	return 0;
}