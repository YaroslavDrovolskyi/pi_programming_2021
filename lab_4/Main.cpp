// 1, 2, 3, 7, 9, 11, 12, 13, 14, 15, 18, 20

#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>

enum Son { left_child, right_child };


void print_path(std::vector<std::size_t> path, std::size_t max_length);
std::size_t get_number_of_digits(int value);


struct Tree;
struct TreeNode;

template <typename Datatype>
struct ListNode {
	ListNode* next;
	Datatype data;

	ListNode(Datatype data, ListNode* next = nullptr) {
		this->data = data;
		this->next = next;
	}
};

template <typename Datatype>
struct Queue {
	ListNode<Datatype>* begin;
	ListNode<Datatype>* end;
	std::size_t size;

	Queue() {
		this->begin = this->end = nullptr;
		this->size = 0;
	}

	void enqueue(Datatype data) {
		ListNode<Datatype>* new_node = new ListNode<Datatype>(data);
		if (this->end) {
			this->end->next = new_node;
		}
		else { // if queue is empty
			this->begin = new_node;
		}
		this->end = new_node;

		this->size++;
	}

	Datatype dequeue() {
		assert(this->begin != nullptr);

		Datatype result = this->begin->data;
		ListNode<Datatype>* current = this->begin;

		this->begin = this->begin->next;
		if (!this->begin) {
			this->end = this->begin;
		}
		delete current;

		this->size--;
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

	/*
	Tree remove_by_path(std::vector<std::size_t>& path) {
		assert(this->root != nullptr);
		TreeNode* node_to_remove = get_by_path(path);
		
		//return remove_in_tree(node_to_remove);
	}
	*/



	void remove_all_by_value(int key, TreeNode* node = nullptr) {
		assert(this->root != nullptr);

		if (node == nullptr) {
			node = this->root;
		}
		
		if (node->data == key) {
			remove_with_deleting(node); // delete node and all its children
		}

		else { // if it isn't key, we need to go on 
			TreeNode* current = node->first_child;
			TreeNode* next_child = (current != nullptr) ? current->next : nullptr;
			
			while (current) {
				remove_all_by_value(key, current);
				current = next_child;

				if (next_child != nullptr) {
					next_child = next_child->next;
				}
				
			}
		}
	}


	Tree remove_one_by_value(int key, TreeNode* node_to_start = nullptr) {
		assert(this->root != nullptr);
		Tree result;
		TreeNode* node_to_remove = search_first_item(key, node_to_start);

		if (node_to_remove){
			std::vector<std::size_t> path = node_to_remove->get_path();
			result = remove_in_tree(path);
		}
		return result; // if node_to_remove == null, then return empty tree
	}


	Tree remove_in_tree(std::vector<std::size_t>& path) {
		assert(this->root != nullptr);
		TreeNode* node_to_remove = get_by_path(path);

		Tree new_tree;
		new_tree.root = node_to_remove;

		disconnect_item(node_to_remove); // disconnect from parent and brothers but not from children

		return new_tree;
	}

	
	

	void print_levels() {
		if (this->root == nullptr) {
			std::cout << "\nTree is empty\n";
		}
		else {
			std::cout << "\nTree by levels:\n";
			TreeNode* const DELIMITER = nullptr;
			Queue<TreeNode*> queue;
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


	void remove_with_deleting(TreeNode* node_to_remove) {
		disconnect_item(node_to_remove);

		while (node_to_remove->first_child) { // firstly remove children and then parent
			remove_with_deleting(node_to_remove->first_child);
		}

		delete node_to_remove;
	}



	private:
	void disconnect_item(TreeNode* node_to_remove) {
		assert(node_to_remove != nullptr);
		if (node_to_remove->parent) {
			if (node_to_remove->next) {
				node_to_remove->next->prev = node_to_remove->prev;
			}
			if (node_to_remove->prev) {
				node_to_remove->prev->next = node_to_remove->next;
			}
			else { // if it is the first child
				node_to_remove->parent->first_child = node_to_remove->next;
			}
		}
		else { // it is root
			assert(this->root == node_to_remove);
			this->root = nullptr;
		}

	}



	TreeNode* search_first_item(int key, TreeNode* node = nullptr) {
		assert(this->root != nullptr);
		if (node == nullptr) {
			node = this->root;
		}

		TreeNode* result = nullptr;

		if (node->data == key) {
			result = node;
		}

		else {
			TreeNode* current = node->first_child;
			
			while (current) { // if node isn't a key and have no children, result doesn't change, so nullptr return
				TreeNode* current_result = nullptr;

				current_result = search_first_item(key, current);

				// chose not nullptr result
				if (current_result != nullptr) {
					result = current_result;
				}
				current = current->next;
			}


		}

		return result;
	}







};



struct BinaryTreeNode {
	int data;
	BinaryTreeNode* parent;
	BinaryTreeNode* left;
	BinaryTreeNode* right;

	BinaryTreeNode(int data, BinaryTreeNode* parent = nullptr, BinaryTreeNode* left = nullptr, BinaryTreeNode* right = nullptr) {
		this->data = data;
		this->parent = parent;
		this->left = left;
		this->right = right;
	}

	void print() {

		std::cout << this->data << "(";
		if (this->left || this->right) {
			if (this->left) {
				this->left->print();
			}
			else {
				std::cout << " ";
			}

			std::cout << ", ";
			if (this->right) {
				this->right->print();
			}
		}
		std::cout << ")";
	}


};

struct BinaryTree {
	BinaryTreeNode* root;

	BinaryTree() {
		this->root = nullptr;
	}

	void add(int data) {
		BinaryTreeNode* new_node = new BinaryTreeNode(data);

		if (this->root == nullptr) {
			this->root = new_node;
		}
		else {
			int child = -1;
			BinaryTreeNode* place = search_place(data, child);
			assert(place != nullptr);

			if (child == left_child) {
				place->left = new_node;
			}
			else if (child == right_child) {
				place->right = new_node;
			}
			new_node->parent = place;
		}
	}

	BinaryTreeNode* search_place(int key, int& son, BinaryTreeNode* node = nullptr) {
		assert(this->root != nullptr);
		if (node == nullptr) {
			node = this->root;
		}
		
		if (key < node->data) {
			if (node->left == nullptr) {
				son = left_child;
				return node;
			}
			else {
				return search_place(key, son, node->left);
			}
			
		}
		else {
			if (node->right == nullptr) {
				son = right_child;
				return node;
			}
			else {
				return search_place(key, son, node->right);
			}
			
		}
	}


	void print() {
		if (this->root == nullptr) {
			std::cout << "\nBinary tree is empty\n";
		}
		else {
			std::cout << "\nBinary tree:\n";
			this->root->print();
			std::cout << std::endl;
		}
	}

	void print_levels() {
		if (this->root == nullptr) {
			std::cout << "\nBinary tree is empty\n";
		}
		else {
			std::cout << "\nBinary tree by levels:\n";
			BinaryTreeNode* const DELIMITER = nullptr;
			Queue<BinaryTreeNode*> queue;
			queue.enqueue(this->root);
			queue.enqueue(DELIMITER);
			while (true) {
				BinaryTreeNode* current = queue.dequeue();
				if (current != DELIMITER) {
					std::cout << current->data << " ";

					if (current->left != nullptr) {
						queue.enqueue(current->left);
					}
					if (current->right != nullptr) {
						queue.enqueue(current->right);
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


struct ThreadNode {
	int data;
	ThreadNode* parent;
	ThreadNode* left;
	ThreadNode* right;
	bool is_treaded;

	ThreadNode(int data, ThreadNode* parent = nullptr, ThreadNode* left = nullptr, ThreadNode* right = nullptr, bool is_threaded = false) {
		this->data = data;
		this->parent = parent;
		this->left = left;
		this->right = right;
		this->is_treaded = is_threaded;
	}

	void print() {

		std::cout << this->data << "(";
		if (this->left || this->right) {
			if (this->left) {
				if (this->left->is_treaded == false) {
					this->left->print();
				}
			}
			else {
				std::cout << " ";
			}

			std::cout << ", ";
			if (this->right) {
				if (this->right->is_treaded == false) {
					this->right->print();
				}
				
			}
		}
		std::cout << ")";
	}

	ThreadNode* get_leftmost() {
		ThreadNode* current = this;
	
		while (current->left) {
			current = current->left;
		}
		return current;
	}

};

struct ThreadedTree {
	ThreadNode* root;

	ThreadedTree() {
		this->root = nullptr;
	}


	void print() {
		if (this->root == nullptr) {
			std::cout << "\nThreaded Binary tree is empty\n";
		}
		else {
			std::cout << "\nThreaded Binary tree:\n";
			this->root->print();
			std::cout << std::endl;
		}
	}

	void print_in_line() {
		if (this->root == nullptr) {
			std::cout << "\nThreaded Binary tree is empty\n";
		}
		else {
			ThreadNode* current = this->root->get_leftmost();

			while (current) {
				std::cout << current->data << " ";
				if (current->is_treaded) {
					current = current->right;
				}
				else {
					if (current->right != nullptr) {
						current = current->right->get_leftmost();
					}
					else {
						current = nullptr;
					}
					
				}
			}
		}
	}

};


ThreadNode* copy_node(BinaryTreeNode* bin_root, ThreadNode* parent) {
	ThreadNode*  thread_root = new ThreadNode(bin_root->data, parent);
	if (bin_root->left != nullptr) {
		thread_root->left = copy_node(bin_root->left, thread_root);
	}
	if (bin_root->right != nullptr) {
		thread_root->right = copy_node(bin_root->right, thread_root);
	}

	return thread_root;

}

ThreadedTree copy_tree(BinaryTree& bin_tree) {
	ThreadedTree new_tree;
	if (bin_tree.root != nullptr) {
		new_tree.root = copy_node(bin_tree.root, nullptr);
	}
	return new_tree;
}


ThreadNode* thread_node(ThreadNode* root) {
	if (root == nullptr) {
		return nullptr;
	}
	if (!root->left && !root->right) {
		return root;
	}

	if (root->left) {
		ThreadNode* left = thread_node(root->left);

		// make thread (leftmost in this subtree -> root of this subtree)
		left->right = root;
		left->is_treaded = true;
	}

	// if root (current node) is rightmost child
	if (root->right == nullptr) {
		return root;
	}

	return thread_node(root->right);
}

// it is the main function (we call it in the program) of threading 
ThreadedTree thread_bin_tree(BinaryTree& bin_tree) {
	ThreadedTree new_tree = copy_tree(bin_tree); // firstly we need to create copy of binary tree

	thread_node(new_tree.root);

	return new_tree;
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

	Tree new_tree = tree.remove_in_tree(path);
	new_tree.print();
	tree.print_levels();



	std::cout << std::endl << "remove all by value:";
	tree.root->first_child->next->first_child->first_child->next->data = 6;
	tree.print();
	tree.remove_all_by_value(1000);
	tree.print();
	tree.print_levels();

	std::cout << std::endl << "remove one by value:";
	Tree new_tree1 = tree.remove_one_by_value(6);
	new_tree1.print();
	tree.print();


	/*
	std::cout << "\n\nRemove a lot:";
	int a = 0;
	path[1] = 2; // 0, 2
	TreeNode* item = tree.get_by_path(path);

	std::cout << "\nStart to add items\n";
	std::cin >> a;
	unsigned int start_time = clock();
	for (std::size_t i = 0; i < 1000; i++) {
		tree.add_by_pointer(1001, item);
	}

	{
		item = item->first_child;

		while (item) {
			for (std::size_t j = 0; j < 1000; j++) {
				tree.add_by_pointer(j, item);
			}
			item = item->next;
		}
		
	}
	
	std::cout << "\nadding time: " << clock() - start_time << std::endl;
	//tree.print();

	std::cout << "\nStart to remove items\n";
	std::cin >> a;
	start_time = clock();
	tree.remove_with_deleting(tree.get_by_path(path));
	tree.print();
	std::cout << "\nremoving time: " << clock() - start_time << std::endl;


	std::cout << "\nExit\n";
	std::cin >> a;
	
	*/

	std::cout << "\n\n\nBinary tree:\n\n";
	BinaryTree bin_tree;
	bin_tree.add(5);
	bin_tree.add(6);
	bin_tree.add(7);
	bin_tree.add(3);
	bin_tree.add(100);
	bin_tree.add(-156);
	bin_tree.add(4);
	bin_tree.add(6);
	bin_tree.print();
	bin_tree.print_levels();


	std::cout << "\n\nBIN and threaded tree:\n\n";
	copy_tree(bin_tree).print();
	bin_tree.print();

	std::cout << "\n\nAfter threating:\n\n";
	bin_tree.add(-170);
	bin_tree.add(5);
	bin_tree.add(5);
	bin_tree.add(93);
	ThreadedTree th_tree = thread_bin_tree(bin_tree);
	th_tree.print_in_line();


	std::cout << "\n\nAfter threating:\n\n";

	BinaryTree bin_empty;
	thread_bin_tree(bin_empty).print_in_line();


	std::system("pause");
	return 0;
}