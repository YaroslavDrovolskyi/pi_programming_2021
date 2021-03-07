#include <iostream>

//using namespace std;

struct ListNode {
	ListNode* next;
	int data;
	std::size_t priority;

	ListNode(int data, std::size_t priority, ListNode* next = nullptr ) {
		this->data = data;
		this->priority = priority;
		this->next = next;
	}
};

struct PriorityQueue {
	ListNode* begin;
	ListNode* end;
	std::size_t size;

	PriorityQueue() {
		this->begin = this->end = nullptr;
		this->size = 0;
	}

	ListNode* place_to_insert(std::size_t priority){
		ListNode* current_node;
		ListNode* place = current_node = this->begin;
		
		while (current_node && priority >= current_node->priority) {
			place = current_node;
			current_node = current_node->next;
		}
		return place;
	}

	void insert(int data, std::size_t priority) {
		if (!this->begin) {
			ListNode* new_node = new ListNode(data, priority);
			this->begin = this->end = new_node;
		}
		if (this->begin->priority > priority) {
			ListNode* new_node = new ListNode(data, priority, this->begin);
			this->begin = new_node;
		}
		else {
			ListNode* place = place_to_insert(priority);
			ListNode* new_node = new ListNode(data, priority);
			if (!place) {
				this->begin = this->end = new_node;

			}
			else {
				new_node->next = place->next;
				place->next = new_node;
			}
		}
		this->size++;
		
	}

	void print() {
		if (!this->begin) {
			std::cout << "Queue is empty" << std::endl;
		}
		else {
			ListNode* current_node = this->begin; 
			while (current_node) {
				std::cout << current_node->priority << '\t' << current_node->data << std::endl;
				current_node = current_node->next;
			}
		}
		std::cout << "Size is: " << this->size << std::endl;
	}


};

int main() {
	PriorityQueue MyQueue;
	MyQueue.insert(98, 5);
	MyQueue.insert(97, 6);
	MyQueue.insert(99, 9);
	MyQueue.insert(34, 5);
	MyQueue.insert(100, 1);
	MyQueue.insert(100, 5);
	MyQueue.insert(95, 7);
	MyQueue.insert(96, 8);
	MyQueue.print();
	std::system("pause");
	return 0;
}