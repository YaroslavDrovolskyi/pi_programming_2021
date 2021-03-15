#include <iostream>
#include <cassert>
#include <vector>

enum find_node_mode {
	removing, inserting
};
enum ProgramMode {
	Exit, Interactive, Demonstrative
};


struct VListNode {
	VListNode* prev;
	VListNode* next;
	std::size_t size;
	std::size_t capacity;
	int* array;

	VListNode(std::size_t capacity, VListNode* prev = nullptr, VListNode* next = nullptr) {
		this->array = new int[capacity];
		this->capacity = capacity;
		this->size = 0;
		this->prev = prev;
		this->next = next;
	}

	// bad way, because it is impossible to change this->end if it isn't enought space in array 
	void push_back(int data) {
		if (this->size == this->capacity) {
			VListNode* new_node = new VListNode(2*this->capacity, this, this->next);
			if (this->next) {
				this->next->prev = new_node;
			}
			this->next = new_node;
			new_node->array[0] = data;
			new_node->size++;
		}
		else {
			this->array[this->size] = data;
			this->size++;
		}

	}

	void insert_in_array(int data_to_insert, std::size_t index_to_insert) {
		if (this->size == this->capacity) {
			for (std::size_t i = this->size - 1; i > index_to_insert; i--) {
				this->array[i] = this->array[i - 1];
			}
		}
		else {
			for (std::size_t i = this->size; i > index_to_insert; i--) {
				this->array[i] = this->array[i - 1];
			}
			this->size++;
		}
		
		this->array[index_to_insert] = data_to_insert;
	}

	void remove_from_array(std::size_t index_to_remove) {
		for (std::size_t i = index_to_remove; i < this->size - 1; i++) {
			this->array[i] = this->array[i + 1];
		}
		this->size--;
	}

};

template <typename ElementType>
bool is_equal(ElementType value_1, ElementType value_2);

struct VList {
	VListNode* begin;
	VListNode* end;
	std::size_t size;  // number of VListNodes
	std::size_t overall_size; // number of elements

	VList() {
		this->begin = this->end = nullptr;
		this->size = 0;
		this->overall_size = 0;
	}
	
	
	void append(int data) {
		if (this->begin == nullptr) {
			create_new(data);
		}
		else {
			if (this->end->size == this->end->capacity) {
				VListNode* new_node = new VListNode(2 * this->end->capacity, this->end);
				this->end->next = new_node;
				this->end = new_node;
				this->size++;
			}
			this->end->array[this->end->size] = data;
			this->end->size++;
		}


		this->overall_size++;
	}


	void insert(int data_to_insert, std::size_t index_to_insert) {
		if (index_to_insert == this->overall_size) { // when list is empty included
			append(data_to_insert);
		}
		else {
			std::size_t index_in_array = 0;
			VListNode* node_to_insert = find_node(index_to_insert, index_in_array, inserting);
			insert_by_pointer(node_to_insert, index_in_array, data_to_insert);
		}

	}


	VListNode* find_node(std::size_t index, std::size_t& index_in_array, std::size_t mode = removing) {
		assert(index < this->overall_size &&  "No ListNode with this index in list"); // this->begin = nullptr included
		std::size_t low_limit = 0, up_limit = this->begin->size;

		VListNode* current = this->begin;
		std::size_t i = 0;
		while (!(index < up_limit && index >=low_limit) && current) {
			
			low_limit += current->size;
			current = current->next;
			i++;
			if (current) {
				up_limit += current->size; // to change up_limit correctly
			}
			
		} 

		index_in_array = index - low_limit;

		if (mode != removing) {
			if (index_in_array == 0 && current->prev) {
				if (current->prev->size != current->prev->capacity) {
					current = current->prev;
					index_in_array = current->size;
				}
			}
		}
		
		return current;
	}

	void remove(std::size_t index_to_remove) {
		assert(index_to_remove < this->overall_size);
		std::size_t index_in_array;
		VListNode* current = find_node(index_to_remove, index_in_array, removing);
		current->remove_from_array(index_in_array);
		if (current->size == 0) {
			remove_node(current);
		}
		this->overall_size--;
	}

	void print() {
		if (this->size == 0) {
			std::cout << "\nList is empty\n";
		}
		else {
			std::cout << "\nYour list:\n";
			VListNode* current = this->begin;
			while (current) {
				for (std::size_t i = 0; i < current->size; i++) {
					std::cout << current->array[i] << ' ';
				}
				std::cout << std::endl;
				current = current->next;
			}
			std::cout << "\nOverall size: " << this->overall_size << std::endl;
			std::cout << "Number of nodes: " << this->size << std::endl;
		}
	}

	void print_reversed() {
		if (this->size == 0) {
			std::cout << "\nList is empty\n";
		}
		else {
			std::cout << "\nYour reversed list:\n";
			VListNode* current = this->end;
			while (current) {
				for (std::size_t i = 0; i <current->size; i++) {
					std::cout << current->array[current->size - i - 1] << ' ';
				}
				//std::cout << std::endl;
				current = current->prev;
			}
			std::cout << "\nOverall size: " << this->overall_size << std::endl;
			std::cout << "Number of nodes: " << this->size << std::endl;
		}
	}


	void clear() {
		if (!this->begin) { return; }
		VListNode* current = this->begin;
		while (this->begin) {
			remove_node(current);
			current = this->begin;
		}
		this->overall_size = 0;
	}

	int get(std::size_t index) {
		assert(index < this->overall_size);
		std::size_t index_in_array;
		VListNode* current = find_node(index, index_in_array);
		return current->array[index_in_array];
	}

	int* get_pointer(std::size_t index) {
		assert(index < this->overall_size);
		std::size_t index_in_array;
		VListNode* current = find_node(index, index_in_array);
		return current->array + index_in_array;
	}



	void set(std::size_t index, int value) {
		assert(index < this->overall_size);
		std::size_t index_in_array;
		VListNode* current = find_node(index, index_in_array);
		current->array[index_in_array] = value;
	}

	std::vector<std::size_t> search(int value) {
		assert(this->begin && "List is empty");
		std::vector<std::size_t> result;
		std::size_t i = 0, j = 0;
		VListNode* current = this->begin;
		while (current) {
			for (j = 0; j < current->size; j++) {
				if (is_equal(value, current->array[j])) {
					result.push_back(i);
				}
				i++;
			}
			current = current->next;
		}
		return result;
	}

private:

	// bad way, because it is impossible to change this->end if it isn't enought space in array 
	void append_and_push(int data) {
		if (this->size == 0) {
			VListNode* new_node = new VListNode(2);
			this->begin = this->end = new_node;
			new_node->array[0] = data;
			new_node->size++;
		}
		else {
			this->end->push_back(data);
		}


		this->size++;
	}

	void create_new(int data) {
		VListNode* new_node = new VListNode(2);
		this->begin = this->end = new_node;
		new_node->array[0] = data;
		new_node->size++;
		this->size++;
	}

	void insert_by_pointer(VListNode* node_to_push, std::size_t index_in_array, int data_to_push) {
		assert(node_to_push != nullptr && "node_to_push is nullptr");
		assert(index_in_array <= node_to_push->size);

		// determination: create new node or not
		bool is_enough_space_in_next = false;
		if (node_to_push->next) {
			if (node_to_push->next->size < node_to_push->next->capacity && node_to_push->next->size <= 10) {
				is_enough_space_in_next = true;
			}
		}

		if (node_to_push->size == node_to_push->capacity) {
			if (is_enough_space_in_next == false) {
				VListNode* new_node = new VListNode(2 * node_to_push->capacity, node_to_push, node_to_push->next);

				if (node_to_push == this->end) {
					this->end = new_node;
				}
				if (node_to_push->next) {
					node_to_push->next->prev = new_node;
				}
				node_to_push->next = new_node;

				new_node->array[0] = node_to_push->array[node_to_push->size - 1]; // Extra element drop in next node
				new_node->size++;
				this->size++;
			}
			else if (is_enough_space_in_next == true) {
				node_to_push->next->insert_in_array(node_to_push->array[node_to_push->size - 1], 0);
			}
		}

		node_to_push->insert_in_array(data_to_push, index_in_array);
		this->overall_size++;
	}

	void remove_node(VListNode* node_to_remove) {
		if (!node_to_remove->next) {
			this->end = node_to_remove->prev;
		}
		else {
			node_to_remove->next->prev = node_to_remove->prev;
		}
		if (!node_to_remove->prev) {
			this->begin = node_to_remove->next;
		}
		else {
			node_to_remove->prev->next = node_to_remove->next;
		}
		delete[] node_to_remove->array;
		delete node_to_remove;
		this->size--;
	}



};



void print_search_result(std::vector<std::size_t> result) {
	if (result.size() == 0) {
		std::cout << "\nNothing was found\n";
	}
	else {
		std::cout << "\nIndexes of wanted elements:\n";
		for (std::size_t i = 0; i < result.size(); i++) {
			std::cout << result[i] << ' ';
		}
		std::cout << std::endl;
	}
}

template <typename ElementType>
void Is_correct_value(ElementType& value_to_check, const int floor_value = -1, const int ceiling_value = -1);

void print_menu();

std::size_t get_index(std::size_t max_index);

int get_value();

int main() {

	/*
	const int N = 25;
	VList List_1, List_2;
//	List_1.insert(0, 0);
	for (std::size_t i = 0; i < N; i++) {
		//List_1.append(i%10);
		List_1.insert(i%6, i);
	}
	List_1.print();
	std::cout << "\n\n\n";
	
	List_1.remove(2);
	List_1.print();
	std::cout << "\n\n\n";

	List_1.remove(0);
	List_1.print();
	std::cout << "\n\n\n";

	List_1.remove(0);
	List_1.print();
	std::cout << "\n\n\n";

	std::cout << "List_1[21] = " << List_1.get(21) << std::endl;

	std::cout << "\n\n\n";

	std::vector<std::size_t> result = List_1.search(5);
	print_search_result(result);
	result.clear();
	result.shrink_to_fit();
	

	List_1.clear();
	List_1.print();
	std::cout << "\n\n\n";
	*/

	VList List;
	std::cout << "Hello!" << std::endl;
	short int program_mode = 0;
	while (true) {
		std::cout << "\nChose program mode (0 - Exit, 1 - Interactive, 2 - Demonstration)\n";
		std::cin >> program_mode;
		Is_correct_value(program_mode, 0, 2);
		if (program_mode == Exit) { break; }
		else if (program_mode == Interactive) {
			short int next;
			while (true) {
				print_menu();
				std::cin >> next;
				Is_correct_value(next, 0, 10);
				if (next == 0) { // choose mode
					List.clear();
					break;
				}
				switch (next) {
				case 1: { // append
					int value_to_append = get_value();
					List.append(value_to_append);
					std::cout << "\nItem successfully appended\n";
					break;
				}
				case 2: { // insert
					
					int value_to_insert = get_value();
					std::size_t index_to_insert = get_index(List.overall_size);

					List.insert(value_to_insert, index_to_insert);
					std::cout << "\nItem successfully inserted\n";
					break;
				}
				case 3: { // get
					if (List.size == 0) { std::cout << "\nList is empty\n"; break; }
					std::size_t index_to_get = get_index(List.overall_size - 1);

					std::cout << "\nYour item is: " << List.get(index_to_get) << std::endl;
					break;
				}
				case 4: { // change item
					if (List.size == 0) { std::cout << "\nList is empty\n"; break; }

					std::size_t index_to_change = get_index(List.overall_size - 1);

					int* old_value = List.get_pointer(index_to_change);
					std::cout << "\nDo you want to change this item? (0 - No, 1 - Yes): " << *old_value;
					short int next = 0;
					std::cin >> next;
					Is_correct_value(next, 0, 1);
					if (next == 0) {
						std::cout << "\nItem not changed\n";
					}
					else {
						int new_value = get_value();
						*old_value = new_value;
						std::cout << "\nItem sucessfully changed\n";
					}
					break;
				}
				case 5: { // remove item
					if (List.size == 0) { std::cout << "\nList is empty\n"; break; }

					std::size_t index_to_remove = get_index(List.overall_size - 1);
					List.remove(index_to_remove);
					std::cout << "Item sucessfull removed\n";
					break;
				}
				case 6: { // search by value
					if (List.size == 0) { std::cout << "\nList is empty\n"; break; }
					
					int value_to_search = get_value();
					std::vector<std::size_t> result = List.search(value_to_search);
					print_search_result(result);
					result.clear();
					result.shrink_to_fit();
					break;
				}
				case 7: { // get size
					std::cout << "\nSize of list = " << List.overall_size << std::endl;
					break;
				}
				case 8: { // print list
					List.print();
					break;
				}
				case 9: { // print reversed list
					List.print_reversed();
					break;
				}
				case 10: { // clear list
					if (List.size == 0) { std::cout << "\nList is already empty\n"; break; }
					List.clear();
					std::cout << "\nList successfully cleared\n";
					break;
				}
				default: break;
				}
			}


		}
		else if (program_mode == Demonstrative) {
			int items[] = { 5, 1, 3, 7, 15, 45, 10, 5, 6, 17, 151, 15, 90, 100 };

			std::cout << "\n1.Append 14 items to list\n";
			for (std::size_t i = 0; i < 14; i++) {
				List.append(items[i]);
			}
			List.print();

			std::cout << "\n2.Insert 1 to the beginning and 5 in the end\n";
			List.insert(1, 0);
			List.insert(5, List.overall_size);
			List.print();

			std::cout << "\n3.Get element with index 5: ";
			std::cout << List.get(5);

			std::cout << "\n4.change element with index 5 on 1000:\n";
			List.set(5, 1000);
			List.print();

			std::cout << "\n5. Remove item with index 9\n";
			List.remove(9);
			List.print();

			std::cout << "\n6. Remove item from beginning and from end\n";
			List.remove(0);
			List.remove(List.overall_size - 1);
			List.print();

			std::cout << "\n7. Search items \"7\":\n";
			std::vector<std::size_t> result = List.search(7);
			print_search_result(result);
			result.clear();
			result.shrink_to_fit();

			std::cout << "\n8. Size of list = " << List.overall_size << std::endl;

			std::cout << "\n9.Print list:\n"; List.print();
			std::cout << "\n10.Print reversed list:\n"; List.print_reversed();

			std::cout << "\n11. Clear list\n";
			List.clear();
			std::cout << "List is sucessfully cleared\n";
			List.print();
			break;
		}
	}

	std::cout << "\n\nTHE END\n\n";

	std::system("pause");
	return 0;
}


template <typename ElementType>
bool is_equal(ElementType value_1, ElementType value_2) {
	if (value_1 == value_2) { return true; }
	return false;
}



template <typename ElementType>
void Is_correct_value(ElementType& value_to_check, const int floor_value, const int ceiling_value) {
	// ceiling value = -1 means that it is no ceil for value_to_check
	// floor value = -1 means that it is no floor for value_to_check
	if (ceiling_value == -1 && floor_value == -1) {
		while (!std::cin.good()) {
			std::cout << "Enter valid value: \n";
			std::cin.clear();
			std::cin.ignore(256, '\n');
			std::cin >> value_to_check;
		}
	}
	else {
		if (ceiling_value == -1) {
			while (value_to_check < floor_value || !std::cin.good()) {
				std::cout << "Enter valid value: \n";
				std::cin.clear();
				std::cin.ignore(256, '\n');
				std::cin >> value_to_check;
			}
		}
		else if (floor_value == -1) {
			while (!std::cin.good()) {
				std::cout << "Enter valid value: \n";
				std::cin.clear();
				std::cin.ignore(256, '\n');
				std::cin >> value_to_check;
			}
		}
		else {
			while (value_to_check < floor_value || value_to_check > ceiling_value || !std::cin.good()) {
				std::cout << "Enter valid value: \n";
				std::cin.clear();
				std::cin.ignore(256, '\n');
				std::cin >> value_to_check;
			}
		}
	}

	return;
}


void print_menu() {
	std::cout << "=====MENU=====\n" <<
		"0 \tchoose mode\n" <<
		"1 \tappend\n" <<
		"2 \tinsert item\n" <<
		"3 \tget item by index\n" <<
		"4 \tchange item by index\n" <<
		"5 \tremove item by index\n" <<
		"6 \tseach by value\n" <<
		"7 \tget size of list\n" <<
		"8 \tprint list\n" <<
		"9 \tprint reversed list\n" <<
		"10 \tclear list\n" <<
		"==============\n\n";
}

std::size_t get_index(std::size_t max_index) {
	std::cout << "Enter index (from 0 to " << max_index << "):";
	std::size_t index = 0;
	std::cin >> index;
	Is_correct_value(index, 0, max_index); // check to not call asserts
	return index;
}

int get_value() {
	std::cout << "Enter value: ";
	int value = 0;
	std::cin >> value;
	Is_correct_value(value);
	return value;
}