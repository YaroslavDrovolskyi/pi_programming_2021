#pragma once

#ifndef LINKED_LIST_
#define LINKED_LIST_

#include <iostream>
#include <cassert>
#include <vector>
#include "Time.h"


struct ListNode {
	Time data;
	ListNode* next;
};



ListNode* create_empty() {
	ListNode* new_node = nullptr;
	return new_node;
}

ListNode* create_node(Time data, ListNode* next = nullptr) {
	ListNode* new_node = new ListNode;
	new_node->data = data;
	new_node->next = next;
	return new_node;
}

void add(ListNode*& list, Time data) {
	ListNode* new_node = create_node(data);
	if (!list) {
		list = new_node;
		return;
	}
	else {
		ListNode* current = list;
		if (data == current->data) { return; }
		if (data < current->data) { // checking if we should insert before first item
			new_node->next = list;
			list = new_node;
			return;
		}

		while (current->next && data > current->next->data) {
			current = current->next;
		}
		if (current->next) {
			if (data < current->next->data) {
				new_node->next = current->next;
				current->next = new_node;
			}
			// else if (data == current->next->data), we don't need to duplicate value
		}
		else { // insert in the end of list
			current->next = new_node;
		}
	}

}

bool remove_node(ListNode*& list, ListNode* node_to_remove) {
	assert(list && "Try to remove from empty list");
	assert(list && "Try to remove nullptr");
	if (list == node_to_remove) {
		list = list->next;
	}
	else {
		ListNode* current = list;
		while (current->next && current->next != node_to_remove) {
			current = current->next;
		}
		assert(current);
		current->next = node_to_remove->next;
	}
	delete node_to_remove;
	return true;
}

bool remove(ListNode*& list, Time data) {
	assert(list && "Try to remove from empty list");
	ListNode* current = list;
	while (current && current->data < data) {
		current = current->next;
	}
	if (!current || current->data != data) { return false; } // if it isn't node with this data
	else {
		return remove_node(list, current);
	}
}

bool check_order(ListNode* list) {
	ListNode* current = list;
	while (current && current->next) {
		if (!(current->data < current->next->data)) { return false; }
		current = current->next;
	}
	return true;
}

ListNode* search(ListNode* list, Time data) {
	if (!list) { return nullptr; }
	ListNode* current = list;
	while (current && current->data < data) {
		current = current->next;
	}
	if (current && current->data == data) { return current; }
	else { return nullptr; }
}

std::vector<ListNode*> search(ListNode* list, Time start, Time end) {
	assert(start <= end && "Wrong search interval");
	std::vector<ListNode*> result;
	ListNode* current = list;
	while (current && current->data <= end) {
		if (current->data >= start && current->data <= end) {
			result.push_back(current);
		}
		current = current->next;
	}
	return result;
}

template<typename Callable>
void process_list(ListNode* list, Callable process) {
	assert(list && "Try to process empty list");
	ListNode* current = list;
	while (current) {
		process(current->data);
		current = current->next;
	}
}

void add_random_items(ListNode*& list, std::size_t size) {
	for (std::size_t i = 0; i < size; i++) {
		add(list, random_time());
	}
}
void clear(ListNode*& list) {
	if (!list) { return; }
	ListNode* current = list;
	ListNode* prev;
	while (current) {
		prev = current;
		current = current->next;
		delete prev;
	}
	list = nullptr;
}
unsigned int get_memory(ListNode* list) {
	ListNode* current = list;
	std::size_t k = 0;
	while (current) {
		k++;
		current = current->next;
	}
	unsigned int memory = sizeof(list) + k * sizeof(ListNode);
	return memory;
}
void print(ListNode* list) {
	if (!list) { std::cout << "\nList is empty\n"; return; }
	ListNode* current = list;
	std::size_t i = 0;
	while (current) {
		std::cout << i << ") " << current->data << std::endl;
		i++;
		current = current->next;
	}
}


#endif LINKED_LIST_