#pragma once

#ifndef QUEUE_H_
#define QUEUE_H_


#include <iostream>
#include <cassert>;

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

#endif /* QUEUE_H_ */