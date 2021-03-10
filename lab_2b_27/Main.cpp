#include <iostream>
#include <cassert>


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

	void insert(int data_to_insert, std::size_t index_to_insert, std::size_t size) {
		if (this->size == this->capacity) {
			for (std::size_t i = size - 1; i > index_to_insert; i--) {
				this->array[i] = this->array[i - 1];
			}
		}
		else {
			for (std::size_t i = size; i > index_to_insert; i--) {
				this->array[i] = this->array[i - 1];
			}
		}
		
		this->array[index_to_insert] = data_to_insert;
	}
};

struct VList {
	VListNode* begin;
	VListNode* end;
	std::size_t size;  // number of VListNodes
	std::size_t overall_size;

	VList() {
		this->begin = this->end = nullptr;
		this->size = 0;
		this->overall_size = 0;
	}
	
	void append(int data) {
		if (this->size == 0) {
			VListNode* new_node = new VListNode(2);
			this->begin = this->end = new_node;
			new_node->array[0] = data;
			new_node->size++;
			this->size++;
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

	VListNode* find_node(std::size_t index) {
		assert(index < this->size && "No ListNode with this index in list"); // this->begin = nullptr included

		VListNode* current = this->begin;
		std::size_t i = 0;
		while (i < index && current) {
			i++;
			current = current->next;
		}
		return current;
	}

	// unfinished
	void push(VListNode* node_to_push, std::size_t index_in_array, int data_to_push) {
		if (this->size == 0) { // is it necessary?
			VListNode* new_node = new VListNode(2);
			this->begin = this->end = new_node;
			new_node->array[0] = data_to_push;
			new_node->size++;
			this->size++;
		}
		else {
			assert(index_in_array < node_to_push->size);
			if (node_to_push->size == node_to_push->capacity) {
				VListNode* new_node = new VListNode(2 * node_to_push->capacity, node_to_push, node_to_push->next);
				
				if (node_to_push == this->end) {
					this->end = new_node;
				}
				if (node_to_push->next) {
					node_to_push->next->prev = new_node;
				}
				node_to_push->next = new_node;

				new_node->array[0] = node_to_push->array[node_to_push->size - 1]; // Extra element drop int next node
				node_to_push->insert(data_to_push, index_in_array, node_to_push->size);
				new_node->size++;
				this->size++;
			}
			else {
				node_to_push->insert(data_to_push, index_in_array, node_to_push->size);
				node_to_push->size++;
			}
			
		}
		this->overall_size++;
	}
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


	void print() {
		if (this->size == 0) {
			std::cout << "\nVList is empty\n";
		}
		else {
			VListNode* current = this->begin;
			while (current) {
				for (std::size_t i = 0; i < current->size; i++) {
					std::cout << current->array[i] << ' ';
				}
				std::cout << std::endl;
				current = current->next;
			}
			std::cout << "\nOverall size: " << this->overall_size << std::endl << 
				"Nunber of nodes: " << this->size << std::endl;
		}
	}
};



int main() {
	const int N = 25;
	VList List_1, List_2;
	for (std::size_t i = 0; i < N; i++) {
		List_1.append(i%10);
	}
	List_1.print();
	std::cout << "\n\n\n";
	//List_1.push(List_1.begin->next->next, 0, 101);
	List_1.push(List_1.begin, 0, 100);
	List_1.push(List_1.begin->next, 0, 101);
	List_1.push(List_1.begin->next, 1, 95);
	List_1.push(List_1.begin->next, 2, 99);
	List_1.print();



	std::system("pause");
	return 0;
}