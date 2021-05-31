#include <iostream>
#include <cassert>




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
void print(ListNode* list);

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
	new_time.month = rand() % 13;
	new_time.day = rand() % 31;
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