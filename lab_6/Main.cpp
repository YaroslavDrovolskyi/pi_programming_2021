#include <iostream>
#include <cassert>
#include <vector>




struct Time {
	short int year;
	short int month;
	short int day;

	short int hours;
	short int minutes;
	short int seconds;

};

struct ListNode {
	Time data;
	ListNode* next;
};

Time random_time();

bool operator== (Time& a, Time& b);
bool operator!= (Time& a, Time& b);
bool operator> (Time& a, Time& b);
bool operator>= (Time& a, Time& b);
bool operator< (Time& a, Time& b);
bool operator<= (Time& a, Time& b);

std::ostream& operator<< (std::ostream& out, Time& time);

ListNode* create_empty();
ListNode* create_node(Time data, ListNode* next = nullptr);
void add(ListNode*& list, Time data);
bool remove_node(ListNode*& list, ListNode* node_to_remove);
bool remove(ListNode*& list, Time data);
bool check_order(ListNode* list);
ListNode* search(ListNode* list, Time data);
std::vector<ListNode*> search(ListNode* list, Time start, Time end);
void print(ListNode* list);
template<typename Callable>
void process_list(ListNode* list, Callable process);
void add_random_items(ListNode*& list, std::size_t size);
unsigned int get_memory(ListNode* list);
void clear(ListNode*& list);

int main() {
	
	ListNode* list = create_empty();
	print(list);
	std::cout << std::endl;
	add(list, Time{ 2007, 05, 15, 20, 45, 23 });
	add(list, Time{ 2020, 05, 15, 20, 45, 23 });
	add(list, Time{ 2007, 05, 15, 20, 45, 23 });
	add(list, Time{ 2000, 05, 15, 20, 45, 23 });
	add(list, Time{ 1975, 05, 15, 20, 45, 23 });
	add(list, Time{ 2005, 05, 15, 20, 45, 23 });
	print(list);
	
	/*
	Time time{ 2005, 05, 15, 20, 45, 23 };
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(list, time);
	print(list);

	time.year = 2007;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(list, time);
	print(list);

	time.year = 1975;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(list, time);
	print(list);

	time.year = 2020;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(list, time);
	print(list);

	time.year = 2000;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(list, time);
	print(list);
	*/

	std::cout << "\n\nSEARCH:" << std::endl << std::endl;
	ListNode* result = search(list, Time{ 1991, 05, 15, 20, 45, 23 });
	if (result) {
		std::cout << result->data << std::endl;
	}
	else {
		std::cout << "\nNot found\n";
	}


	std::cout << "\n\nSEARCH in interval:" << std::endl << std::endl;
	std::vector<ListNode*> result_v = search(list, Time{ 1800, 05, 15, 20, 45, 23 }, Time{ 2000, 05, 15, 20, 45, 22 });
	if (result_v.size() > 0) {
		for (std::size_t i = 0; i < result_v.size(); i++) {
			std::cout << result_v[i]->data << std::endl;
		}
	}
	else {
		std::cout << "\nNot found\n";
	}


	short int year_summa = 0;
	process_list(list, [&](Time& time) {std::cout << time.year << std::endl; });
	std::cout << "\n\n\nSumma of years: " << year_summa << std::endl;

	add_random_items(list, 10);
	print(list);

	std::cout << "\n\n\nMemory: " << get_memory(list) << std::endl;

	clear(list);
	print(list);
	std::system("pause");
	return 0;
}


bool operator> (Time& a, Time& b) {
	if (a.year > b.year) { return true; }
	else if (a.year == b.year) {
		if (a.month > b.month) { return true; }
		else if (a.month == b.month) {
			if (a.day > b.day) { return true; }
			else if (a.day == b.day) {
				if (a.hours > b.hours) { return true; }
				else if (a.hours == b.hours) {
					if (a.minutes > b.minutes) { return true; }
					else if (a.minutes == b.minutes) {
						if (a.seconds > b.seconds) { return true; }
						else if (a.seconds == b.seconds) {
							return false;
						}
					}
				}
			}
		}
	}
	return false;
}

bool operator== (Time& a, Time& b) {
	if (a.year == b.year && a.month == b.month && a.day == b.day && a.hours == b.hours && a.minutes == b.minutes && a.seconds == b.seconds) { return true; }
	return false;
}

bool operator!= (Time& a, Time& b) {
	return !(a == b);
}

bool operator>= (Time& a, Time& b) {
	return (a > b || a == b);
}

bool operator< (Time& a, Time& b) {
	return !(a > b || a == b);
}

bool operator<= (Time& a, Time& b) {
	return (a < b || a == b);
}

std::ostream& operator<< (std::ostream& out, Time& time) {
	out << time.day << "." << time.month << "." << time.year << ", " <<
		time.hours << ":" << time.minutes << ":" << time.seconds;
	return out;
}

Time random_time() {
	Time new_time;
	new_time.year = rand() % 46 + 1975;
	new_time.month = rand() % 12 + 1;
	new_time.day = rand() % 30 + 1 ;
	new_time.hours = rand() % 24;
	new_time.minutes = rand() % 60;
	new_time.seconds = rand() % 60;

	return new_time;
}

ListNode* create_empty() {
	ListNode* new_node = nullptr;
	return new_node;
}

ListNode* create_node(Time data, ListNode* next) {
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

void add_random_items(ListNode*&list, std::size_t size) {
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
		std::cout  << i << ") " << current->data << std::endl;
		i++;
		current = current->next;
	}
}