#include <iostream>
#include <string>
#include <cassert>

//using namespace std;

enum Program_Mode {
	Exit, Interactive, Demonstration 
};

struct Time {
	short int hour, minutes, seconds;
};
struct Date {
	short int day, month, year;
};
struct Message {
	Date date;
	Time time;
	std::string text;
};

struct ListNode {
	ListNode* prev;
	ListNode* next;
	Message data;

	ListNode(Message data, ListNode* prev = nullptr, ListNode* next = nullptr) {
		this->data = data;
		this->prev = prev;
		this->next = next;
	}
};

void print_message(Message message);

template <typename ElementType>
void Is_correct_value(ElementType& value_to_check, const int floor_value = -1, const int ceiling_value = -1);
void print_menu();
Message get_message();
int get_index();

// this program realization based on cyclic list
struct MessageLog {
	ListNode* begin;
	ListNode* end;
	std::size_t size;
	std::size_t capacity;

	MessageLog(std::size_t capacity) {
		this->begin = this->end = nullptr;
		this->size = 0;
		this->capacity = capacity;
	}

	// With removing a first element
	void push_with_removing(Message new_data) { // works slowly - 957 ms to add 100000 new elements
		ListNode* new_node = new ListNode(new_data, this->end, this->begin);
		if (!this->begin) { // if list is empty
			this->begin = this->end = new_node;
		}
		else if (this->capacity == this->size) {
			ListNode* current = this->begin->next;
			remove_node(this->begin);
			if (this->size == 0) {
				new_node->next = new_node->prev = nullptr;
				this->begin = this->end = new_node;
			}
			else {
				new_node->prev = this->end;
				this->begin = current;
				this->end->next = new_node;
				this->end = new_node;
				this->end->next = this->begin;
				this->begin->prev = this->end;
			}
		}
			
		else {
			this->end->next = this->begin->prev = new_node;
			this->end = new_node;
		}
		this->size++;
	}

	void push(Message new_data) { // 604 ms to add 100000 new elements
		if (this->capacity == this->size) {
			this->begin->data = new_data;
			if (this->size != 1) {
				this->end = this->end->next;
				this->begin = this->begin->next;
			}
		}
		else {
			ListNode* new_node = new ListNode(new_data, this->end, this->begin);
			if (!this->begin) { // if list is empty
				this->begin = this->end = new_node;
				this->size++;
			}
			else {
				this->end->next = this->begin->prev = new_node;
				this->end = new_node;
				this->size++;
			}
		}
		
	
	}

	

	void print() {
		if (this->size == 0) {
			std::cout << "\nMesage log is empty\n";
		}
		else {
			std::cout << "\nYour message log is (index, date, time, message text):\n";
			ListNode* current = this->begin;
			for (std::size_t i = 0; i < this->size; i++) {
				std::cout << i << '\t';
				print_message(current->data);
				current = current->next;
			}
			std::cout << "list size = " << this->size << std::endl;
			std::cout << "list capacity = " << this->capacity << std::endl << std::endl;
		}
	}

	ListNode* search_by_index(std::size_t index_to_search) {
		assert(index_to_search < this->size && "No this index in list");
		ListNode* current = this->begin;
		std::size_t i = 0;
		while (i < index_to_search) {
			current = current->next;
			i++;
		}
		return current;
	}
	void remove_by_index(std::size_t index_to_remove) {
		assert(index_to_remove < this->size && "Trying to remove non-existent item");
		remove_node(search_by_index(index_to_remove));
	}
	Message* last_messages(short int number) {
		assert(number <= this->size && "Try to get more items, than exist");
		Message* result = new Message[number];
		ListNode* current = this->end;
		for (std::size_t i = 1; i <= number; i++) { // fill container from the end
			result[number - i] = current->data;
			current = current->prev;
		}
		return result;
	}
	void clear() {
		ListNode* current = this->begin;
		for (std::size_t i = 0; i < this->size; i++) {
			this->begin = this->begin->next;
			delete current;
			current = this->begin;
		}
		this->begin = this->end = nullptr;
		this->size = 0;
	}

private:

	void remove_node(ListNode* node_to_remove) {
		assert(this->size > 0 && "Try to remove item from empty list");
		if (this->size == 1) { // if 1 item in list
			this->begin = this->end = nullptr;
		}
		else {
			if (this->size != 2) {
				if (node_to_remove == this->begin) {
					this->begin = node_to_remove->next;
				}
				else if (node_to_remove == this->end) {
					this->end = node_to_remove->prev;
				}
				node_to_remove->next->prev = node_to_remove->prev;
				node_to_remove->prev->next = node_to_remove->next;
			}
			else { // if size == 2
				if (node_to_remove == this->begin) {
					this->begin = this->end;
					this->begin->next = nullptr;
					this->begin->prev = nullptr;
				}
				else if (node_to_remove == this->end) {
					this->end = this->begin;
					this->begin->next = nullptr;
					this->begin->prev = nullptr;
				}
			}
			node_to_remove->next->prev = node_to_remove->prev;
			node_to_remove->prev->next = node_to_remove->next;
		}
		delete node_to_remove;
		this->size--;

	}


};




int main() {
	MessageLog MyList(0);
	std::cout << "Hello!" << std::endl;
	short int program_mode = 0;
	while (true) {
		std::cout << "\nChose program mode (0 - Exit, 1 - Interactive, 2 - Demonstration)\n";
		std::cin >> program_mode;
		Is_correct_value(program_mode, 0, 2);

		if (program_mode == Exit) { break; }
		else if (program_mode == Interactive) {

			int list_capacity = 0;
			std::cout << "\nEnter a maximum size (capacity) of message log (>0): ";
			std::cin >> list_capacity;
			Is_correct_value(list_capacity, 1);
			MyList.capacity = list_capacity;

			short int next;
			while (true) {
				print_menu();
				std::cin >> next;
				Is_correct_value(next, 0, 6);
				if (next == 0) {
					MyList.clear();
					MyList.capacity = 0;
					break;
				}
				switch (next) {
				case 1: { // add item
					MyList.push(get_message());
					std::cout << "\nMessage was successfully added to message log\n";
					break;
				}
				case 2: { // remove by index
					if (MyList.size == 0) { // check for don't call assert in function
						std::cout << "\nMesage log is empty\n";
					}
					else {
						std::size_t index_to_remove = get_index();
						if (index_to_remove >= MyList.size) {
							std::cout << "\nNo this index in message log\n";
						}
						else {
							MyList.remove_by_index(index_to_remove);
							std::cout << "\nItem removed from message log\n";
						}
						
					}
					break;
				}
				case 3: { // get last k messages
					if (MyList.size == 0) { // check for don't call assert in function
						std::cout << "\nMesage log is empty\n";
					}
					else {
						int K;
						std::cout << "\nEnter K (from 1 to " << MyList.size << "): ";
						std::cin >> K;
						Is_correct_value(K, 1, MyList.size);
						Message* result = MyList.last_messages(K);
						std::cout << "\n\n" << K << " last items:\n";
						for (std::size_t i = 0; i < K; i++) {
							print_message(result[i]);
						}
						delete[]result;
					}
					break;
				}
				case 4: { // size and capacity
					std::cout << "\nSize of message log = " << MyList.size << std::endl;
					std::cout << "Capacity of message log = " << MyList.capacity << std::endl;
					break;
				}
				case 5: { // print list
					MyList.print();
					break;
				}
				case 6: { // clear
					if (MyList.size == 0) {
						std::cout << "\nMessage log is already empty\n";
					}
					else {
						MyList.clear();
						std::cout << "\nMessage log is successfully cleared\n";
					}
					break;
				}
				
				default: break;
				}
			}


		}
		else if (program_mode == Demonstration) {
			std::cout << "\n1. Set the message log capacity = 5\n";
			MyList.capacity = 5;
			std::cout << "Message log capacity = " << MyList.capacity << std::endl;

			std::cout << "\n2. Fill it by 5 items\n";
			MyList.push({ {10,10,2020}, {23,23,23}, "Message_1" });
			MyList.push({ {11,10,2020}, {21,20,22}, "Message_2" });
			MyList.push({ {12,10,2020}, {19,50,55}, "Message_3" });
			MyList.push({ {13,10,2020}, {16,51,11}, "Message_4" });
			MyList.push({ {14,10,2020}, {17,25,15}, "Message_5" });
			MyList.print();

			std::cout << "\n3. Add 3 items\n";
			MyList.push({ {10,10,2020}, {20,20,20}, "Message_6" });
			MyList.push({ {20,10,2020}, {18,31,95}, "Message_7" });
			MyList.push({ {12,10,2020}, {12,25,10}, "Message_8" });
			MyList.print();

			std::cout << "\n4. Remove item with index 2 (middle item)\n";
			MyList.remove_by_index(2);
			MyList.print();

			std::cout << "\n5. Remove item with index 0 (first item)\n";
			MyList.remove_by_index(0);
			MyList.print();

			std::cout << "\n6. Remove item with index 2 (last item)\n";
			MyList.remove_by_index(2);
			MyList.print();

			std::cout << "\n7. Add 3 items\n";
			MyList.push({ {10,9,2020}, {20,20,20}, "New_Message_1" });
			MyList.push({ {20,8,2020}, {18,31,05}, "Mew_Message_2" });
			MyList.push({ {12,10,2020}, {12,25,10}, "New_Message_3" });
			MyList.print();

			std::cout << "\n8. Get last 4 messages:\n";
			Message* result = MyList.last_messages(4);
			std::cout << "\n\n" << 4 << " last items:\n";
			for (std::size_t i = 0; i < 4; i++) {
				print_message(result[i]);
			}
			delete[]result;

			std::cout << "\n9. Print size and capacity of message log:\n";
			std::cout << "\nSize of message log = " << MyList.size << std::endl;
			std::cout << "Capacity of message log = " << MyList.capacity << std::endl;

			std::cout << "\n10. Clear message log:\n";
			MyList.clear();
			MyList.print();
			break;
		}
	}


	std::cout << "\n\nThe END!!!!\n\n";
					

	std::system("pause");
	return 0;
}

void print_message(Message message) {
	std::cout << message.date.day << '.' << message.date.month << '.' << message.date.year << '\t';
	std::cout << message.time.hour << ':' << message.time.minutes << ':' << message.time.seconds << '\t';
	std::cout << message.text << std::endl;
}

Message get_message() {
	Message new_message;
	std::cout << "\nEneter a message:\n";
	std::cin.ignore(256, '\n');
	getline(std::cin, new_message.text);
	

	//cout << "Enter a date:\n";
	std::cout << "Enter a day:  ";
	std::cin >> new_message.date.day; Is_correct_value(new_message.date.day, 1, 31);

	std::cout << "Enter a month:  ";
	std::cin >> new_message.date.month; Is_correct_value(new_message.date.month, 1, 12);

	std::cout << "Enter a year:  ";
	std::cin >> new_message.date.year; Is_correct_value(new_message.date.year, 1);

	std::cout << "Enter a time hours:  ";
	std::cin >> new_message.time.hour; Is_correct_value(new_message.time.hour, 0, 23);

	std::cout << "Enter a time minutes:  ";
	std::cin >> new_message.time.minutes; Is_correct_value(new_message.time.minutes, 0, 59);

	std::cout << "Enter a time seconds:  ";
	std::cin >> new_message.time.seconds; Is_correct_value(new_message.time.seconds, 0, 59);

	return (new_message);

}

int get_index() { // index is integer because it is the one way to ckeck if the user value <0 or >0
	std::cout << "\nEnter index\n";
	int index;
	std::cin >> index;
	Is_correct_value(index, 0);
	return index;
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
			while (value_to_check - floor_value < 0 || !std::cin.good()) {
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
			while (value_to_check - floor_value < 0 || value_to_check - ceiling_value > 0 || !std::cin.good()) {
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
	std::cout << "\n\n=====MENU=====\n" <<
		"0 \tchoose mode\n" <<
		"1 \tadd new item to message log\n" <<
		"2 \tremove message by index\n" <<
		"3 \tget last K messages\n" <<
		"4 \tprint size and capacity of message log\n" <<
		"5 \tprint message log\n" <<
		"6 \tclear message log\n" <<
		"==============\n\n";
}