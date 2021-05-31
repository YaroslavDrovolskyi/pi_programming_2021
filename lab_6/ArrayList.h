#pragma once

#ifndef ARRAYLIST_
#define ARRAYLIST_

#include <iostream>
#include <cassert>
#include <vector>

#include "Time.h"

struct ArrayList {
	Time* array;
	std::size_t size;
	std::size_t capacity;
};

std::vector<Time> create_aray_list() {
	std::vector<Time> array_list;
	return array_list;
}

ArrayList create_array_list() {
	ArrayList list;
	list.array = nullptr;
	list.size = 0;
	list.capacity = 0;

	return list;
}

void increase_capacity(ArrayList& list) {
	Time* new_arr = new Time[2 * list.capacity];
	for (std::size_t i = 0; i < list.capacity; i++) {
		new_arr[i] = list.array[i];
	}
	delete[]list.array;
	list.array = new_arr;
	list.capacity *= 2;
}

void push_back(ArrayList& list, Time data) {
	if (list.array == nullptr) {
		list.array = new Time[5];
		list.size = 0;
		list.capacity = 5;		
	}
	else if (list.size == list.capacity) {
		increase_capacity(list);
	}
	list.array[list.size] = data;
	list.size++;
}

void insert(ArrayList& list, std::size_t index, Time data) {
	assert(index < list.size);
	if (list.size == list.capacity) {
		increase_capacity(list);
	}
	for (std::size_t i = list.size; i != index; i--) {
		list.array[i] = list.array[i - 1];
	}
	list.size++;
	list.array[index] = data;
}

void erase(ArrayList& list, std::size_t index) {
	assert(index < list.size);
	for (std::size_t i = index; i < list.size - 1; i++) {
		list.array[i] = list.array[i + 1];
	}
	list.size--;
}

/* based on own Array list realization*/

void add(ArrayList& list, Time data) {
	if (list.size == 0) { push_back(list, data); }
	else {
		if (data == list.array[0]) { return; }
		if (data < list.array[0]) {
			insert(list, 0, data);
			return;
		}
		std::size_t i = 0;
		while (i < list.size && data > list.array[i]) {
			i++;
		}
		if (i < list.size) {
			if (data < list.array[i]) {
				insert(list, i, data);
			}
			// else we don't need to duplicate value
		}
		else {
			push_back(list, data);
		}
	}
}

bool remove(ArrayList& list, Time data) {
	std::size_t i = 0;
	while (i < list.size && list.array[i] < data) {
		i++;
	}
	if (i < list.size) {
		erase(list, i);
		return true;
	}
	else {
		return false;
	}
}

Time* search(ArrayList& list, Time data) {
	std::size_t i = 0;
	while (i < list.size && list.array[i] < data) {
		i++;
	}
	if (i < list.size && list.array[i] == data) { return &list.array[i]; }
	else { return nullptr; }
}

std::vector<Time*> search(ArrayList& list, Time start, Time end) {
	assert(start <= end && "Wrong search interval");
	std::vector<Time*> result;
	std::size_t i = 0;
	while (i < list.size && list.array[i] <= end) {
		if (list.array[i] >= start && list.array[i] <= end) {
			result.push_back(&list.array[i]);
		}
		i++;
	}
	return result;
}

template<typename Callable>
void process_list(ArrayList& list, Callable process) {
	assert(list.size > 0 && "Try to process empty list");
	for (std::size_t i = 0; i < list.size; i++) {
		process(list.array[i]);
	}
}

void add_random_items(ArrayList& list, std::size_t size) {
	for (std::size_t i = 0; i < size; i++) {
		add(list, random_time());
	}
}

unsigned int get_memory(ArrayList& list) {
	unsigned int memory = sizeof(ArrayList) + list.capacity * sizeof(Time);
	return memory;
}

void clear(ArrayList& list) {
	delete[] list.array;
	list.array = nullptr;
	list.size = 0;
	list.capacity = 0;
}



bool check_order(ArrayList& list) {
	if (list.size == 0) { return true; }
	for (std::size_t i = 0; i < list.size - 1; i++) {
		if (!(list.array[i] < list.array[i + 1])) { return false; }
	}
	return true;
}

void print(ArrayList& list) {
	if (list.size == 0) { std::cout << "\nList is empty\n"; return; }
	for (std::size_t i = 0; i < list.size; i++) {
		std::cout << i << ") " << list.array[i] << std::endl;
	}
}

/****   Based on std::vector  ****/
/* 
void add(std::vector<Time>& list, Time data) {
	if (list.size() == 0) { list.push_back(data); }
	else {
		if (data == list[0]) { return; }
		if (data < list[0]) {
			list.insert(list.begin(), data);
			return;
		}
		std::size_t i = 0;
		while (i < list.size() && data > list[i]) {
			i++;
		}
		if (i < list.size()) {
			if (data < list[i]) {
				list.insert(list.begin() + i, data);
			}
			// else we don't need to duplicate value
		}
		else {
			list.push_back(data);
		}
	}
}

bool remove(std::vector<Time>& array_list, Time data) {
	std::size_t i = 0;
	while (i < array_list.size() && array_list[i] < data) {
		i++;
	}
	if (i < array_list.size()) {
		array_list.erase(array_list.begin() + i);
		return true;
	}
	else {
		return false;
	}
}

Time* search(std::vector<Time>& array_list, Time data) {
	std::size_t i = 0;
	while (i < array_list.size() && array_list[i] < data) {
		i++;
	}
	if (i < array_list.size() && array_list[i] == data) { return &array_list[i]; }
	else { return nullptr; }
}

std::vector<Time*> search(std::vector<Time>& array_list, Time start, Time end) {
	assert(start <= end && "Wrong search interval");
	std::vector<Time*> result;
	std::size_t i = 0;
	while (i < array_list.size() && array_list[i] <= end) {
		if (array_list[i] >= start && array_list[i] <= end) {
			result.push_back(&array_list[i]);
		}
		i++;
	}
	return result;
}

template<typename Callable>
void process_list(std::vector<Time>& array_list, Callable process) {
	assert(array_list.size() > 0 && "Try to process empty list");
	for (std::size_t i = 0; i < array_list.size(); i++) {
		process(array_list[i]);
	}
}

void add_random_items(std::vector<Time>& array_list, std::size_t size) {
	for (std::size_t i = 0; i < size; i++) {
		add(array_list, random_time());
	}
}

void clear(std::vector<Time>& array_list) {
	array_list.clear();
	array_list.shrink_to_fit();
}


unsigned int get_memory(std::vector<Time>& array_list) {
	unsigned int memory = sizeof(std::vector<Time>) + array_list.capacity() * sizeof(Time);
	return memory;
}

bool check_order(std::vector<Time>& array_list) {
	if (array_list.size() == 0) { return true; }
	for (std::size_t i = 0; i < array_list.size() - 1; i++) {
		if (!(array_list[i] < array_list[i + 1])) { return false; }
	}
	return true;
}

void print(std::vector<Time>& array_list) {
	if (array_list.size() == 0) { std::cout << "\nList is empty\n"; return; }
	for (std::size_t i = 0; i < array_list.size(); i++) {
		std::cout << array_list[i] << std::endl;
	}
}

*/





#endif ARRAYLIST_