// 1, 2, 3, 7, 9, 11, 12, 13, 14, 15, 18, 20, 26

#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>
#include <string>

enum Son { left_child, right_child };
enum type { file, folder };

struct Time {
	short int year;
	short int month;
	short int day;
	short int hour;
	short int minutes;
	short int seconds;
};

struct Directory {
	std::string name;
	std::size_t size;
	Time edit_time;
	short int type;

};

std::ostream& operator<< (std::ostream& out, Directory& directory) {
	out << directory.name << ", " << directory.size << " bytes, ";
	if (directory.type == folder) {
		out << "folder, ";
	}
	else {
		out << "file, ";
	}
	out << directory.edit_time.hour << ":" << directory.edit_time.minutes << ":" << directory.edit_time.seconds << "  "
		<< directory.edit_time.day << "." << directory.edit_time.month << "." << directory.edit_time.year << "   ";

	return out;
}

std::ostream& operator << (std::ostream& out, Time& time) {
	out << time.hour << ":" << time.minutes << ":" << time.seconds << "  "
		<< time.day << "." << time.month << "." << time.year;

	return out;
}

bool operator < (Time& time_1, Time& time_2) {
	if (time_1.year < time_2.year) { return true; }
	else if (time_1.year == time_2.year) {
		if (time_1.month < time_2.month) { return true; }
		else if (time_1.month == time_2.month) {
			if (time_1.day < time_2.day) { return true; }
			else if (time_1.day == time_2.day) {
				if (time_1.hour < time_2.hour) { return true; }
				else if (time_1.hour == time_2.hour) {
					if (time_1.minutes < time_2.minutes) { return true; }
					else if (time_1.minutes == time_2.minutes) {
						if (time_1.seconds < time_2.seconds) { return true; }
						else if (time_1.seconds == time_2.seconds) { return false; }
					}
				}
			}
		}
	}

	return false;
}

bool operator > (Time& time_1, Time& time_2) {
	if (time_1.year > time_2.year) { return true; }
	else if (time_1.year == time_2.year) {
		if (time_1.month > time_2.month) { return true; }
		else if (time_1.month == time_2.month) {
			if (time_1.day > time_2.day) { return true; }
			else if (time_1.day == time_2.day) {
				if (time_1.hour > time_2.hour) { return true; }
				else if (time_1.hour == time_2.hour) {
					if (time_1.minutes > time_2.minutes) { return true; }
					else if (time_1.minutes == time_2.minutes) {
						if (time_1.seconds > time_2.seconds) { return true; }
						else if (time_1.seconds == time_2.seconds) { return false; }
					}
				}
			}
		}
	}

	return false;
}


void print_path(std::vector<std::size_t> path, std::size_t max_length);
std::size_t get_number_of_digits(int value);



template <typename T> struct Tree;
template <typename T> struct TreeNode;


//template<typename T>
// void filtration_node(TreeNode<T>* head, Tree<T>& new_tree, bool (*criterion)(TreeNode<T>));


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



template <typename T>
struct TreeNode {
	T data;
	TreeNode* parent;
	TreeNode* first_child;
	TreeNode* next;
	TreeNode* prev;

	TreeNode(T data, TreeNode* parent = nullptr, TreeNode* first_child = nullptr, TreeNode* next = nullptr, TreeNode* prev = nullptr) {
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

		//std::cout << "\nInteractive print ended\n";
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

template <typename T>
struct Tree {
	TreeNode<T>* root;

	Tree() {
		this->root = nullptr;
	}


	TreeNode<T>* get_by_path(std::vector<std::size_t>& path) {
		assert(this->root != nullptr);
		if (path.size() == 0) { return this->root; }
		TreeNode<T>* current = this->root;
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

	void add_by_path(T data, std::vector<std::size_t>& path) {
		if (path.size() == 0 && !this->root) {
			add_by_pointer(data, nullptr);
		}
		else if (path.size() == 0 && this->root) {
			add_by_pointer(data, this->root);
		}
		else {
			add_by_pointer(data, get_by_path(path));
		}

	}


	void add_by_pointer(T data, TreeNode<T>* parent = nullptr) {
		TreeNode<T>* new_node = new TreeNode<T>(data, parent);
		if (parent == nullptr) {
			assert(this->root == nullptr);
			this->root = new_node;
		}
		else {
			TreeNode<T>* current = parent->first_child; // parent won't be NULL

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

	std::vector<std::size_t> get_path(TreeNode<T>* node) {
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
		TreeNode<T>* current = this->root;
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
					TreeNode<T>* current_child = current->first_child;
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



	void remove_all_by_value(T key, TreeNode<T>* node = nullptr) {
		assert(this->root != nullptr);

		if (node == nullptr) {
			node = this->root;
		}
		
		if (node->data == key) {
			remove_with_deleting(node); // delete node and all its children
		}

		else { // if it isn't key, we need to go on 
			TreeNode<T>* current = node->first_child;
			TreeNode<T>* next_child = (current != nullptr) ? current->next : nullptr;
			
			while (current) {
				remove_all_by_value(key, current);
				current = next_child;

				if (next_child != nullptr) {
					next_child = next_child->next;
				}
				
			}
		}
	}


	Tree remove_one_by_value(T key, TreeNode<T>* node_to_start = nullptr) {
		assert(this->root != nullptr);
		Tree result;
		TreeNode<T>* node_to_remove = search_first_item(key, node_to_start);

		if (node_to_remove){
			std::vector<std::size_t> path = node_to_remove->get_path();
			result = remove_in_tree(path);
		}
		return result; // if node_to_remove == null, then return empty tree
	}


	Tree remove_in_tree(std::vector<std::size_t>& path) {
		assert(this->root != nullptr);
		TreeNode<T>* node_to_remove = get_by_path(path);

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
			TreeNode<T>* const DELIMITER = nullptr;
			Queue<TreeNode<T>*> queue;
			queue.enqueue(this->root);
			queue.enqueue(DELIMITER);
			while (true) {
				TreeNode<T>* current = queue.dequeue();
				if (current != DELIMITER) {
					std::cout << current->data << " ";

					TreeNode<T>* current_child = current->first_child;
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


	void remove_with_deleting(TreeNode<T>* node_to_remove) {
		disconnect_item(node_to_remove);

		while (node_to_remove->first_child) { // firstly remove children and then parent
			remove_with_deleting(node_to_remove->first_child);
		}

		delete node_to_remove;
	}

	template <typename Callable>
	/*   bool (*criterion)(TreeNode<T>*)   */
	Tree<T> filtration(Callable criterion) {
		Tree<T> new_tree;
		filtration_node <T, Callable>(this->root, new_tree, criterion);

		return new_tree;
	}



	private:
	void disconnect_item(TreeNode<T>* node_to_remove) {
		assert(node_to_remove != nullptr && "Try to disconnect nullptr item");
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



	TreeNode<T>* search_first_item(T key, TreeNode<T>* node = nullptr) {
		assert(this->root != nullptr);
		if (node == nullptr) {
			node = this->root;
		}

		TreeNode<T>* result = nullptr;

		if (node->data == key) {
			result = node;
		}

		else {
			TreeNode<T>* current = node->first_child;
			
			while (current) { // if node isn't a key and have no children, result doesn't change, so nullptr return
				TreeNode<T>* current_result = nullptr;

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


unsigned int get_directory_size(TreeNode<Directory>* head) {
	if (head == nullptr) { return 0; }

	unsigned int size = head->data.size;
	TreeNode<Directory>* current = head->first_child;
	while (current) {
		size += get_directory_size(current);
		current = current->next;
	}

	return size;
}

// for using neccessary to create folders_number, files_number and initialize it by 0
void get_folders_number(TreeNode<Directory>* head, std::size_t& folders_number, std::size_t& files_number) {
	if (head == nullptr) {
		return;
	}

	if (head->data.type == folder) {
		folders_number++;
	}
	else {
		files_number++;
	}

	TreeNode<Directory>* current = head->first_child;
	while (current) {
		get_folders_number(current, folders_number, files_number);
		current = current->next;
	}


}


// for using neccessary to create min_time, max_time and initialize it by root->data.edit_time
void get_max_min_time(TreeNode<Directory>* head, Time& min_time, Time& max_time) {
	if (head == nullptr) { return; }

	if (head->data.edit_time < min_time) {
		min_time = head->data.edit_time;
	}

	if (head->data.edit_time > max_time) {
		max_time = head->data.edit_time;
	}

	TreeNode<Directory>* current = head->first_child;
	while (current) {
		get_max_min_time(current, min_time, max_time);
		current = current->next;
	}
}

Time get_time() {
	Time time;
	std::cout << "Enter changing time (hh mm ss, dd mm yy):";
	std::cin >> time.hour >> time.minutes >> time.seconds
		>> time.day >> time.month >> time.year;

	return time;
}



Directory get_directory_item() {
	Directory new_item;
	std::cout << "Enter name of folder or file\n";
	std::cin.ignore(256, '\n');
	getline(std::cin, new_item.name);
	std::cout << "Enter size: ";
	std::cin >> new_item.size;
	std::cout << "Enter type (0 - file, 1 - folder): ";
	std::cin >> new_item.type;
	
	new_item.edit_time = get_time();

	return new_item;
}


std::vector<std::size_t> get_path() {
	std::vector<std::size_t> path;
	std::cout << "\nEnter path (-1 to stop) (empty - to add in root (if tree is empty) and as a child of root (if tree is not empty)): \n";
	int current = 1;
	while (current >= 0) {
		std::cin >> current;
		if (current >= 0) {
			path.push_back(current);
		}
	}
	return path;
}

template<typename T, typename Callable>
/*   bool (*criterion)(TreeNode<T>* node)   */
void filtration_node(TreeNode<T>* head, Tree<T>& new_tree, Callable criterion) {
	if (head == nullptr) { return; }
	if (criterion(head) == true) {
		new_tree.add_by_pointer(head->data, new_tree.root);
	}
	TreeNode<T>* current = head->first_child;
	while (current) {
		filtration_node(current, new_tree, criterion);
		current = current->next;
	}
}

bool time_criterion(TreeNode<Directory>* node, Time time) {
	return node->data.edit_time > time;
}

bool type_criterion(TreeNode<Directory>* node, short int type) {
	return node->data.type == type;
}

bool size_criterion(TreeNode<Directory>* node, unsigned int size) {
	return node->data.size > size;
}


void add_in_directory(Tree<Directory>& tree, Directory new_item, std::vector<std::size_t> path) {
	if (tree.root == nullptr && path.size() == 0 && new_item.type != file) { // it is only one possible variant to add item in empty directory tree
		tree.add_by_path(new_item, path);
	}
	else if (tree.root == nullptr && path.size() == 0 && new_item.type == file) {
		std::cout << "\nImpossible to put file into a root of directory\n";
	}
	else {
		TreeNode<Directory>* place = tree.get_by_path(path);
		assert(place != nullptr);
		assert(place->data.type != file && "Try to add directory in file");
		tree.add_by_pointer(new_item, place);
	}

}

// function for more readable outpu of directory tree in demonstrative mode
void print_inorder(TreeNode<Directory>* head) {
	if (head == nullptr) {
		return;
	}
	std::cout << head->data << std::endl;
	TreeNode<Directory>* current = head->first_child;
	while (current) {
		print_inorder(current);
		current = current->next;
	}

}

// can't demonstrate function of getting path by pointer for user, because user can't enter the pointer 
void interactive() {
	Tree<int> tree;
	BinaryTree bin_tree;
	ThreadedTree threaded_tree;
	Tree<Directory> directory_tree;
	//std::vector<std::size_t> path;
	short int next = 1;
	std::cout << "\nHello!\n";
	while (next > 0) {
		std::cout << "\n============\n" <<
			"0\t exit\n" <<
			"1\t add item by path\n" <<
			"2\t get item by path\n" <<
			"3\t print tree with brackets\n" <<
			"4\t print tree by levels\n" <<
			"5\t print tree with paths of items\n" <<
			"6\t interactive print of tree\n" <<
			"7\t remove item by path and return its subtree\n" <<
			"8\t remove item by value and return its subtree\n" <<
			"9\t remove all items by value and clear a memory\n\n" <<
			"10\t add items to a binary tree\n" <<
			"11\t print binary tree with brackets\n" <<
			"12\t print binary tree by levels\n" <<
			"13\t build threaded binary tree according to symmetrical order\n\n" <<
			"14\t add item in directory by path\n" <<
			"15\t interactive print\n" <<
			"16\t remove directory and return it\n" <<
			"17\t get directory size\n" <<
			"18\t get number of files and folders\n" <<
			"19\t get min and max changing time\n" <<
			"20\t filtration directory by time (> fixed time)\n" <<
			"21\t filtration directory (folders ot files)\n" <<
			"22\t filtration directory by size (> fixed size)\n" <<
			"========================\n\n\n";
		std::cin >> next;
		switch (next) {
			case 1: {
				int data;
				std::cout << "Enter a data: ";
				std::cin >> data;
				std::vector<std::size_t> path = get_path();
				tree.add_by_path(data, path);
				std::cout << "Item succesfully added\n";
				break;
			}
			case 2: {
				std::vector<std::size_t> path = get_path();
				std::cout << "Item is: " << tree.get_by_path(path)->data << std::endl;
				break;
			}
			case 3: {
				tree.print();
				break;
			}
			case 4: {
				tree.print_levels();
				break;
			}
			case 5: {
				tree.print_with_path();
				break;
			}
			case 6: {
				tree.interactive_print();
				break;
			}
			case 7: {
				std::vector<std::size_t> path = get_path();
				std::cout << "\nRemoved subtree:\n";
				tree.remove_in_tree(path).print();
				break;
			}
			case 8: {
				int value = 0;
				std::cout << "\nEnter a value: ";
				std::cin >> value;
				std::cout << "\nRemoved subtree (if it is empty, so no your value in tree):\n";
				tree.remove_one_by_value(value).print();
				break;
			}
			case 9: {
				int value = 0;
				std::cout << "\nEnter a value: ";
				std::cin >> value;
				tree.remove_all_by_value(value);
				std::cout << "\nItems removed (if they were in tree)!\n";
				break;
			}
			case 10: {
				int value = 0;
				std::cout << "\nEnter a value: ";
				std::cin >> value;
				bin_tree.add(value);
				std::cout << "\nItem successfully added\n";
				break;
			}
			case 11: {
				bin_tree.print();
				break;
			}
			case 12: {
				bin_tree.print_levels();
				break;
			}
			case 13: {
				thread_bin_tree(bin_tree).print_in_line();
				break;
			}
			case 14: {
				Directory new_item = get_directory_item();
				std::vector<std::size_t> path = get_path();
				add_in_directory(directory_tree, new_item, path);
				break;
			}
			case 15: {
				directory_tree.interactive_print();
				break;
			}
			case 16: {
				std::vector<std::size_t> path = get_path();
				std::cout << "\nRemoved directory:\n";
				directory_tree.remove_in_tree(path).interactive_print();
				break;
			}
			case 17: {
				std::cout << "\nsize of directory: " << get_directory_size(directory_tree.root) << std::endl;
				break;
			}
			case 18: {
				std::size_t folders = 0, files = 0;
				get_folders_number(directory_tree.root, folders, files);
				std::cout << "\nfolders: " << folders << std::endl <<
					"files: " << files << std::endl;
				break;
			}
			case 19: {
				
				if (directory_tree.root == nullptr) {
					std::cout << "\nDirectory is empty\n";
				}
				else {
					Time min = directory_tree.root->data.edit_time, max = min;
					get_max_min_time(directory_tree.root, min, max);
					std::cout << "\nmin time: " << min << std::endl <<
						"max time: " << max << std::endl;
				}
				break;
			}
			case 20: {
				Time time = get_time();
				Tree<Directory> tree_filtrated = directory_tree.filtration([time](TreeNode<Directory>* node) { return time_criterion(node, time); });
				tree_filtrated.print_levels();
				break;
			}
			case 21: {
				short int type;
				std::cout << "Enter type (0 - files, 1 - folders): ";
				std::cin >> type;
				Tree<Directory> tree_filtrated = directory_tree.filtration([type](TreeNode<Directory>* node) { return type_criterion(node, type); });
				tree_filtrated.print_levels();
				break;
			}
			case 22: {
				unsigned int size;
				std::cout << "Enter size (in bytes): ";
				std::cin >> size;
				Tree<Directory> tree_filtrated = directory_tree.filtration([size](TreeNode<Directory>* node) { return size_criterion(node, size); });
				tree_filtrated.print_levels();
				break;
			}
			default: {
				break;
			}
		}
	}
	return;
}


void demonstrative() {

	std::vector<std::size_t> path;

	std::cout << "\n====== Tree ======\n\n";
	Tree<int> tree;
	
	std::cout << "\n1. Add some items by path\n";
	tree.add_by_path(5, path);
	tree.add_by_path(6, path);
	tree.add_by_path(45, path);
	
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
	tree.add_by_path(92, path);

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


	path[1] = 0;
	std::cout << "\n2. Item by path (1, 0) = " << tree.get_by_path(path)->data << std::endl;
	

	std::cout << "\n3. Print tree with brackets:\n";
	tree.print();


	std::cout << "\n4. Print tree by level:\n";
	tree.print_levels();

	std::cout << "\n5. Print tree with paths:\n";
	tree.print_with_path();

	std::cout << "\n6. remove item by path (1, 0) and return its subtree:\n";
	tree.remove_in_tree(path).print();

	std::cout << "\n8. remove item by value = 95 and return its subtree:\n";
	tree.remove_one_by_value(95).print();


	std::cout << "\n9. remove all items by value = 92 and clear a memory:\n";
	tree.remove_all_by_value(92);
	tree.print();

	std::cout << "\n\n\n====== Binary tree ======\n\n";
	BinaryTree bin_tree;
	std::cout << "\n10. Add some items to binary tree:\n";
	bin_tree.add(5);
	bin_tree.add(3);
	bin_tree.add(6);
	bin_tree.add(4);
	bin_tree.add(-156);
	bin_tree.add(-170);
	bin_tree.add(7);
	bin_tree.add(6);
	bin_tree.add(100);

	std::cout << "\n11. print binary tree with brackets:\n";
	bin_tree.print();
	std::cout << "\n12. print binary tree by levels:\n";
	bin_tree.print_levels();
	std::cout << "\n13. build threaded binary tree according to symmetrical order and print it in line:\n";
	thread_bin_tree(bin_tree).print_in_line();

	std::cout << "\n\n\n====== Directory tree ======\n\n";
	Tree<Directory> directory_tree;
	std::cout << "14. add items in directory by path and print it by inorder:\n";
	path.clear();
	directory_tree.add_by_path({ "name_1", 100, {2020, 1, 21, 15, 35, 59}, folder }, path);
	directory_tree.add_by_path({ "name_2", 150, {2021, 1, 24, 15, 45, 01}, folder }, path);
	directory_tree.add_by_path({ "name_3", 120, {2014, 1, 24, 15, 33, 59}, file }, path);
	directory_tree.add_by_path({ "name_4", 200, {2016, 1, 24, 15, 31, 58}, folder }, path);

	path.push_back(0); // 0
	directory_tree.add_by_path({ "name_5", 100, {2020, 1, 21, 15, 35, 45}, folder }, path);
	directory_tree.add_by_path({ "name_6", 150, {2001, 1, 24, 16, 59, 01}, folder }, path);
	directory_tree.add_by_path({ "name_7", 120, {2015, 1, 24, 01, 56, 06}, file }, path);

	path.push_back(1); // 0, 1
	directory_tree.add_by_path({ "name_8", 300, {2015, 1, 24, 22, 51, 07}, file }, path);

	path.clear();
	path.push_back(2); // 1
	directory_tree.add_by_path({ "name_9", 200, {2017, 1, 24, 18, 46, 33}, folder }, path);
	directory_tree.add_by_path({ "name_10", 200, {2014, 1, 24, 12, 55, 34}, file }, path);

	//directory_tree.print_levels();
	print_inorder(directory_tree.root);
	std::cout << "\n15. remove directory by path (1) and return it:\n";
	directory_tree.remove_in_tree(path).print_levels();

	std::cout << "\n16. Directory size = " << get_directory_size(directory_tree.root) << std::endl;

	std::cout << "\n17. get number of files and folders:\n";
	std::size_t folders = 0, files = 0;
	get_folders_number(directory_tree.root, folders, files);
	std::cout << "\nfolders: " << folders << std::endl <<
		"files: " << files << std::endl;

	std::cout << "\n18. get min and max changing time:\n";
	if (directory_tree.root == nullptr) {
		std::cout << "\nDirectory is empty\n";
	}
	else {
		Time min = directory_tree.root->data.edit_time, max = min;
		get_max_min_time(directory_tree.root, min, max);
		std::cout << "\nmin time: " << min << std::endl <<
			"max time: " << max << std::endl;
	}

	std::cout << "\n19. filtration directory by time (> fixed time = 18:46:33, 24.01.2015):\n";
	Time time{ 2015, 1, 24, 18, 46, 33 };
	Tree<Directory> tree_filtrated = directory_tree.filtration([time](TreeNode<Directory>* node) { return time_criterion(node, time); });
	//tree_filtrated.print_levels();
	print_inorder(tree_filtrated.root);

	std::cout << "\n20. filtration directory (folders):\n";
	short int type = 1;
	tree_filtrated = directory_tree.filtration([type](TreeNode<Directory>* node) { return type_criterion(node, type); });
	print_inorder(tree_filtrated.root);



	std::cout << "\n21. filtration directory by size (> fixed size = 100 bytes):\n";

	unsigned int size = 100;
	tree_filtrated = directory_tree.filtration([size](TreeNode<Directory>* node) { return size_criterion(node, size); });
	//tree_filtrated.print_levels();
	print_inorder(tree_filtrated.root);

	std::cout << "\n\n\n Demonstrative mode ended\n\n\n\n";

}




int main() {
	short int next = 0;
	std::cout << "\nHello!\n0 - exit, 1 - interactive mode, 2 - demonstrative mode\n";
	std::cin >> next;
	if (next == 0) {
		return 0;
	}
	else if (next == 1) {
		interactive();
	}
	else if (next == 2) {
		demonstrative();
	}
	std::system("pause");
	return 0;
}