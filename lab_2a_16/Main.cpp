#include <iostream>
#include <string>
#include <ctime>

//using namespace std;

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
			this->end = this->end->next;
			this->begin = this->begin->next;
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

	void remove_node(ListNode* node_to_remove) {
		if (!node_to_remove->next) { // 1 item
			this->begin = this->end = nullptr;
		}
		else {
			if (node_to_remove == this->begin) {
				if (this->size != 2) {
					this->begin = node_to_remove->next;
				}
				else {
					this->begin = this->end;
					this->begin->next = nullptr;
					this->begin->prev = nullptr;
				}
			}
			else if (node_to_remove == this->end) {
				if (this->size != 2) {
					this->end = node_to_remove->prev;
				}
				else {
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

	void print() {
		if (this->size == 0) {
			std::cout << "\nList is empty\n";
		}
		else {
			std::cout << "\nYour list is:\n";
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

};




int main() {
	MessageLog MyLog(3);
	/*
	MyLog.push({ {23,23,23}, "Message_1" });
	MyLog.push({ {20,20,20}, "Message_2" });
	MyLog.push({ {10,23,23}, "Message_3" });
	MyLog.push({ {23,23,23}, "Message_4" });
	MyLog.push({ {23,23,23}, "Message_5" });
	MyLog.print();
	*/

	/*
	unsigned long int start_time = clock();
	for (std::size_t i = 0; i < 10000; i++) {
		MyLog.push({ {23,23,23}, "Message_1" });
		MyLog.push({ {20,20,20}, "Message_2" });
		MyLog.push({ {10,23,23}, "Message_3" });
		MyLog.push({ {23,23,23}, "Message_4" });
		MyLog.push({ {23,23,23}, "Message_5" });
		MyLog.push({ {23,23,23}, "Message_6" });
		MyLog.push({ {23,23,23}, "Message_7" });
		MyLog.push({ {23,23,23}, "Message_8" });
		MyLog.push({ {23,23,23}, "Message_9" });
		MyLog.push({ {23,23,23}, "Message_10" });
	}
	std::cout <<"\nWithout removing: "<< (double)(clock() - start_time) / 1000 << std::endl;
	start_time = clock();
	for (std::size_t i = 0; i < 10000; i++) {
		MyLog.push_with_removing({ {23,23,23}, "Message_1" });
		MyLog.push_with_removing({ {20,20,20}, "Message_2" });
		MyLog.push_with_removing({ {10,23,23}, "Message_3" });
		MyLog.push_with_removing({ {23,23,23}, "Message_4" });
		MyLog.push_with_removing({ {23,23,23}, "Message_5" });
		MyLog.push_with_removing({ {23,23,23}, "Message_6" });
		MyLog.push_with_removing({ {23,23,23}, "Message_7" });
		MyLog.push_with_removing({ {23,23,23}, "Message_8" });
		MyLog.push_with_removing({ {23,23,23}, "Message_9" });
		MyLog.push_with_removing({ {23,23,23}, "Message_10" });
	}
	std::cout << "\nWith removing: " << (double)(clock() - start_time) / 1000 << std::endl << std::endl;
	
	MyLog.print();
	*/



	std::system("pause");
	return 0;
}

void print_message(Message message) {
	std::cout << message.time.hour << ':' << message.time.minutes << ':' << message.time.seconds << '\t';
	std::cout << message.text << std::endl;
}

Message get_message() {
	Message new_message;
	std::cout << "\nEneter a message:\n";
	std::cin.ignore(256, '\n');
	getline(std::cin, new_message.text);
	

	//cout << "Enter a date in format DD MM YY:  ";
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