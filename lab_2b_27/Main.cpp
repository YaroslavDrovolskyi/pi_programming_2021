#include <iostream>


struct VListNode {
	VListNode* prev;
	VListNode* next;
	std::size_t size;
	std::size_t capacity;
	int* data;

	VListNode(std::size_t capacity, VListNode* prev = nullptr, VListNode* next = nullptr) {
		this->data = new int[capacity];
		this->capacity = capacity;
		this->size = 0;
		this->prev = prev;
		this->next = next;
	}

};

struct VList {
	VListNode* begin;
	VListNode* end;
	std::size_t size;

	VList() {
		this->begin = this->end = nullptr;
		this->size = 0;
	}
};



int main() {




	std::system("pause");
	return 0;
}