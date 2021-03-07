#include <iostream>
#include <cassert>

//using namespace std;

enum Program_Mode {
	Exit, Interactive, Demonstrative
};

template <typename DataType>
struct ListNode {
	ListNode<DataType>* next;
	DataType data;
	std::size_t priority;

	ListNode<DataType>(DataType data, std::size_t priority, ListNode* next = nullptr) {
		this->data = data;
		this->priority = priority;
		this->next = next;
	}
};

template <typename DataType>
struct PriorityQueue {
	ListNode<DataType>* begin;
	ListNode<DataType>* end;
	std::size_t size;

	PriorityQueue() {
		this->begin = this->end = nullptr;
		this->size = 0;
	}



	void insert(DataType data, std::size_t priority) {
		ListNode<DataType>* new_node = new ListNode<DataType>(data, priority);
		if (!this->begin) {
			this->begin = this->end = new_node;
		}
		else {
			if (this->begin->priority > priority) {
				new_node->next = this->begin;
				this->begin = new_node;
			}
			else {
				ListNode<DataType>* place = place_to_insert(priority);
				if (!place) { // this is duplication, not very optimal
					this->begin = this->end = new_node;
				}
				else {
					new_node->next = place->next;
					place->next = new_node;
				}
			}
		}
		this->size++;
	}
	DataType dequeu() {
		assert(this->begin != nullptr && "Try get item from empty queue");

		DataType result = this->begin->data;
		ListNode<DataType>* current_node = this->begin;
		this->begin = this->begin->next;
		delete current_node;

		if (!this->begin) { // if queue is empty now after pop item
			this->end = nullptr;
		}
		this->size--;
		return result;
	}
	int peek() {
		assert(this->begin != nullptr && "Try get item from empty queue");
		return this->begin->data;
	}
	void print() {
		if (!this->begin) {
			std::cout << "Queue is empty" << std::endl;
		}
		else {
			ListNode<DataType>* current_node = this->begin;
			while (current_node) {
				std::cout << current_node->priority << '\t' << current_node->data << std::endl;
				current_node = current_node->next;
			}
		}
		std::cout << "Size is: " << this->size << std::endl;
	}

	void clear() {
		ListNode<DataType>* current_node = this->begin;
		while (this->begin) {
			this->begin = this->begin->next;
			delete current_node;
			current_node = this->begin;
		}
		this->end = nullptr;
		this->size = 0;
	}


private:
	ListNode<DataType>* place_to_insert(std::size_t priority) {
		ListNode<DataType>* current_node;
		ListNode<DataType>* place = current_node = this->begin;

		while (current_node && priority >= current_node->priority) {
			place = current_node;
			current_node = current_node->next;
		}
		return place;
	}

};

template <typename ElementType>
void Is_correct_value(ElementType& value_to_check, const int floor_value = -1, const int ceiling_value = -1);
std::size_t get_priority();
int get_value();
void print_menu();


int main() {
	PriorityQueue<double> MyQueue;
	/*
	MyQueue.insert(1.3, 5);
	MyQueue.insert(1.3, 6);
	MyQueue.insert(1.3, 9);
	MyQueue.insert(2.3, 5);
	MyQueue.insert(1.3, 0);
	MyQueue.insert(1.3, 1);
	MyQueue.insert(3.3, 5);
	MyQueue.insert(2.3, 0);
	MyQueue.insert(1.3, 7);
	MyQueue.insert(1.3, 8);
	MyQueue.print();
	//std::cout << "\n\n\n\n\n";
	for (std::size_t i = 1; i <= 10; i++) {
		std::cout << MyQueue.dequeu() << std::endl;
	}
	std::cout << std::endl << MyQueue.size << std::endl;
	//std::cout << MyQueue.dequeu() << std::endl;
	*/


	std::cout << "Hello!" << std::endl;
	short int program_mode = 0;
	while (true) {
		std::cout << "\nChose program mode (1 - Interactive, 2 - Demonstration\n";
		std::cin >> program_mode;
		Is_correct_value(program_mode, 0, 2);
		if (program_mode == Exit) { break; }
		else if (program_mode == Interactive) {
			short int next;
			while (true) {
				print_menu();
				std::cin >> next;
				Is_correct_value(next, 0, 5);
				if (next == 0) { break; }
				switch (next) {
				case 1: { // add item
					MyQueue.insert(get_value(), get_priority());
					std::cout << "\nItem inserted\n";
				}
				case 2: { // pop

				}
				case 3: { // peek

				}
				case 4: { // size

				}
				case 5: { // clear

				}
				default:break;
				}
			}


		}
		else if (program_mode == Demonstrative) {

		}
	}

	std::cout << "\n\n\nTHE END!!!\n\n";
	std::system("pause");
	return 0;
}






template <typename ElementType>
void Is_correct_value(ElementType& value_to_check, const int floor_value, const int ceiling_value) {
	// ceiling value = -1 means that it is no ceil for value_to_check
	// floor value = -1 means that it is no floor for value_to_check
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
	return;
}


std::size_t get_priority() {
	std::cout << "\nEnter a priority: ";
	std::size_t priority;
	std::cin >> priority;
	Is_correct_value(priority, 0);
	return priority;
}

int get_value() {
	std::cout << "\nEnter a value: ";
	int value;
	std::cin >> value;
	Is_correct_value(value);
	return value;
}

void print_menu() {
	std::cout << "=====MENU=====" <<
		"0 \tchoose mode\n" <<
		"1 \tadd item to queue\n" <<
		"2 \tpop\n" <<
		"3 \tpeek\n" <<
		"3 \tsize of queue\n" <<
		"5 \tremove queue\n" <<
		"==============\n\n";
}