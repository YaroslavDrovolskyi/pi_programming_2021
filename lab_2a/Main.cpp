#include <iostream>

using namespace std;

enum ProgramMode { Exit, Interactive, Demonstration, Benchmark };


struct Point {
	float x, y, z;
};

template <typename ElementType>
struct StaticArrayList {
	ElementType* array;
	size_t size;
	size_t capacity;
};


template <typename ElementType>
struct ListNode {
	ListNode* next;
	ListNode* prev;
	ElementType data;
};

template <typename ElementType>
struct DoublyLinkedList {
	ListNode<ElementType>* begin;
	ListNode<ElementType>* end;
	size_t size;
};

template <typename ElementType>
DoublyLinkedList<ElementType>* create_empty();

template <typename ElementType>
void append(DoublyLinkedList <ElementType>* list, ElementType value);

template <typename ElementType>
ListNode<ElementType>* insert(DoublyLinkedList<ElementType>* list, size_t index_to_insert, ElementType value);

template <typename ElementType>
short int remove(DoublyLinkedList<ElementType>* list, size_t index_to_remove);

template <typename ElementType>
ListNode<ElementType>* get(DoublyLinkedList<ElementType>* list, size_t index_to_search);

template <typename ElementType>
size_t length(DoublyLinkedList<ElementType>* list);

template <typename ElementType>
void Is_correct_value(ElementType& value_to_check, const int floor_value = -1, const int ceiling_value = -1);

Point get_point();

int get_index();
void print_Menu();

template <typename ElementType>
void print_List(DoublyLinkedList<ElementType>* list);

void print_Point(Point p);

template <typename ElementType>
ListNode<ElementType>* insert(DoublyLinkedList<ElementType>* list, size_t index_to_insert, ElementType value);

void StaticArray_Realization();



template <typename ElementType>
StaticArrayList<ElementType> create_empty(const size_t SIZE);

template <typename ElementType>
short int append(StaticArrayList<ElementType>& list, const ElementType value);

template <typename ElementType>
short int insert(StaticArrayList<ElementType>& list, const size_t index_to_insert, const ElementType value);

template <typename ElementType>
short int remove(StaticArrayList<ElementType>& list, const size_t index_to_remove);

template <typename ElementType>
ElementType* get(StaticArrayList<ElementType>& list, const size_t index_to_get);

template <typename ElementType>
size_t length(StaticArrayList<ElementType>& list);

template <typename ElementType>
void print_List(StaticArrayList<ElementType>& list);




int main() {
	
	DoublyLinkedList<Point>* MyList = nullptr;
	bool is_list_created = false;
	
	cout << "Hello!\n\n";
	short int program_mode = 1, next = 1;
	while (program_mode) {
		cout << "\nChoose the mode\n" <<
			"0 - EXIT\n" <<
			"1 - Interactive mode\n" <<
			"1 - Demonstrative\n" <<
			"3 - Benchark\n\n";
		cin >> program_mode;
		Is_correct_value(program_mode, 0, 3);
		if (program_mode == Interactive) {
			cout << "\nChoose the realization of list (1 -- linked list, 2 -- static array, 3 -- std::vector): ";
			short int realization;
			cin >> realization;
			Is_correct_value(realization, 1, 3);
			if (realization == 1) { // linked list
				while (true) {
					print_Menu();
					short int next;
					cin >> next;
					Is_correct_value(next, 0, 7);
					if (next == 0) { break; }
					switch (next) {
					case 1: {
						if (MyList) {
							cout << "\nList is already created\n";
							break;
						}
						MyList = create_empty <Point>();
						is_list_created = true;
						cout << "\nList is created\n";
						break;
					}
					case 2: { // add in the end
						if (!MyList) {
							cout << "List isn't created";
						}
						else {
							cout << "\nEnter item:\n";
							//Point new_point = get_point();
							append(MyList, get_point());
							cout << "\nItem added\n";
						}
						break;
					}
					case 3: { // insert by index
						if (!MyList) {
							cout << "\nList isn't created\n";
						}
						else {
							int index_to_insert;
							cout << "\nEnter index: ";
							cin >> index_to_insert;
							Is_correct_value(index_to_insert, 0);
							if (insert(MyList, index_to_insert, get_point())) {
							cout << "\nItem succesfully inserted\n";
							}
							else {
								cout << "\nNo element with this index\n";
							}
						}
						break;
					}
					case 4: { // remove by index
						if (!MyList) {
							cout << "\nList isn't created\n";
							break;
						}
						else {
							int index_to_remove = get_index();
							if (remove(MyList, index_to_remove) == 0) {
								cout << "\nNo element with this index\n";
							}
							else {
								cout << "\nItem successfully removed\n";
							}
						
						}
						break;
					}
					case 5: { // get by index
						int index_to_search = get_index();
						ListNode<Point>* result_of_search = get(MyList, index_to_search);
						if (result_of_search) {
							cout << "\nYour item is: ";
							print_Point(result_of_search->data);
						}
						else {
							cout << "\nNo element with this index\n";
						}
						break;
					}
					case 6: { // length
						if (!MyList) {
							cout << "\nList is empty\n";
						}
						else {
							cout << "Length of linked list is: " <<
								length(MyList) << endl;
						}
						break;
					}
					case 7: { // print list
						if (!MyList) {
							cout << "\nList isn't created\n";
						}
						else {
							print_List(MyList);
						}
						break;
					}
					default: break;
					}
				}

			}
			else if (realization == 2) { // static array
			StaticArray_Realization();
			}
			else if (realization == 3) { // std::vector

			}
		}
		else if (program_mode == Demonstration) {
			break;
		}

		else if (program_mode == Benchmark) {
			break;
		}
	}
	cout << "\n\n\nTHE end!\n\n\n";
	system("pause");
	return 0;
}




template <typename ElementType>
short int remove(DoublyLinkedList<ElementType>* list, size_t index_to_remove) {
	ListNode<ElementType>* node_to_remove = get(list, index_to_remove);

	if (node_to_remove == nullptr) {
		return 0;
	}

	if (node_to_remove->prev) {
		node_to_remove->prev->next = node_to_remove->next;
	}
	else {
		list->begin = node_to_remove->next;
	}
	if (node_to_remove->next) {
		node_to_remove->next->prev = node_to_remove->prev;
	}
	else {
		list->end = node_to_remove->prev;
	}
	delete node_to_remove;
	list->size--;
	return 1;
}



template <typename ElementType>
ListNode<ElementType>* insert(DoublyLinkedList<ElementType>* list, size_t index_to_insert, ElementType value) {
	ListNode<ElementType>* new_node = new ListNode<ElementType>;
	ListNode<ElementType>* current_node = get(list, index_to_insert);
	if (current_node == nullptr) {
		return nullptr;
	}
	new_node->data = value;

	new_node->next = current_node;
	new_node->prev = current_node->prev; // for NULL also ok
	current_node->prev = new_node;

	if (current_node == list->begin) {
		list->begin = new_node;
	}
	else {
		new_node->prev->next = new_node;
	}

	list->size++;
	return new_node;
}

template <typename ElementType>
ListNode<ElementType>* get(DoublyLinkedList<ElementType>* list, size_t index_to_search) {
	if (index_to_search >= list->size) {
		return nullptr;
	}
	ListNode<ElementType>* current;
	size_t i;
	if (index_to_search < (list->size - 1) / 2) {
		current = list->begin;
		i = 0;
		while (current) {
			if (i == index_to_search) {
				return current;
			}
			i++;
			current = current->next;
		}
	}
	else {
		current = list->end;
		i = list->size - 1;
		while (current) {
			if (i == index_to_search) {
				return current;
			}
			i--;
			current = current->prev;
		}
	}
	return current;
}


template <typename ElementType>
DoublyLinkedList<ElementType>* create_empty() {
	DoublyLinkedList<ElementType>* new_list = new DoublyLinkedList<ElementType>;
	new_list->begin = new_list->end = nullptr;
	new_list->size = 0;
	return new_list;
}

template <typename ElementType>
void append(DoublyLinkedList <ElementType>* list, ElementType value) {
	ListNode<ElementType>* new_node = new ListNode <ElementType>;
	new_node->data = value;
	new_node->next = nullptr;
	new_node->prev = list->end;
	if (list->size == 0) {
		list->begin = new_node;
	}
	else {
		list->end->next = new_node;
	}
	list->end = new_node;
	list->size++;
}

template <typename ElementType>
size_t length(DoublyLinkedList<ElementType>* list) {
	return list->size;
}




template <typename ElementType>
void Is_correct_value(ElementType& value_to_check, const int floor_value, const int ceiling_value) {
	// ceiling value = -1 means that it is no ceil for value_to_check
	// floor value = -1 means that it is no floor for value_to_check
	if (ceiling_value == -1) {
		while (value_to_check - floor_value < 0 || !cin.good()) {
			cout << "Enter valid value: \n";
			cin.clear();
			cin.ignore(256, '\n');
			cin >> value_to_check;
		}
	}
	else if (floor_value == -1) {
		while (!cin.good()) {
			cout << "Enter valid value: \n";
			cin.clear();
			cin.ignore(256, '\n');
			cin >> value_to_check;
		}
	}
	else {
		while (value_to_check - floor_value < 0 || value_to_check - ceiling_value > 0 || !cin.good()) {
			cout << "Enter valid value: \n";
			cin.clear();
			cin.ignore(256, '\n');
			cin >> value_to_check;
		}
	}
	return;
}

Point get_point() {
	Point user_point;
	cout << "Enter a point coordinate X: ";
	cin >> user_point.x;
	Is_correct_value(user_point.x);

	cout << "Enter a point coordinate Y: ";
	cin >> user_point.y;
	Is_correct_value(user_point.y);

	cout << "Enter a point coordinate Z: ";
	cin >> user_point.z;
	Is_correct_value(user_point.z);

	return user_point;
}

int get_index() {
	int index;
	cout << "\nEnter index: ";
	cin >> index;
	Is_correct_value(index, 0);
	return index;
}

template <typename ElementType>
void print_List(DoublyLinkedList<ElementType>* list) {
	if (list->begin == nullptr) {
		cout << "\nList is empty\n";
		return;
	}
	int i = 0;
	ListNode<ElementType>* current_node = list->begin;
	cout << endl << "Your list is: \n";
	while (current_node) {
		i++;
		cout << i << '\t';
		print_Point(current_node->data);
		current_node = current_node->next;
	}
}

void print_Point(Point p) {
	cout << '(' << p.x << "; " << p.y << "; " << p.z << ")\n";
	return;
}

void print_Menu() {
	cout << "\n\n=======MENU=======\n" <<
		"0 \tchoose mode\n" <<
		"1 \tcreate new list\n" <<
		"2 \tadd in the list end\n" <<
		"3 \tinsert in list by index\n" <<
		"4 \tremove element by index\n" <<
		"5 \tget element by index\n" <<
		"6 \tlength of list\n" <<
		"7 \tprint list\n" <<
		"===================\n\n";
}

void StaticArray_Realization(){
	const size_t SIZE = 20;
	StaticArrayList<Point> StaticList;
	StaticList.array = nullptr;
	

	while (true) {
		print_Menu();
		short int next;
		cin >> next;
		Is_correct_value(next, 0, 7);
		switch (next) {
		case 0: { // only way to turn back
			return;
			break;
		}
		case 1: { // create list
			if (!StaticList.array) {

				Point arr[SIZE];
				StaticList.array = arr;
				StaticList.size = 0;
				StaticList.capacity = SIZE;

				cout << "\nList successfully created\n";
			}
			else {
				cout << "\nList is already created\n";
			}
			break;
		}
		case 2: { // append
			if (StaticList.array == nullptr) {
				cout << "\nList isn't created\n";
			}
			else {
				if (append(StaticList, get_point()) == 1) {
					cout << "\nSucessfully appended\n";
				}
				else {
					cout << "\nError of append: List is already full\n";
				}
			}
			break;
		}
		case 3: { // insert by index
			if (StaticList.array == nullptr) {
				cout << "\nList isn't created\n";
			}
			else {
				size_t index_to_insert = get_index();
				Point value_to_insert = get_point();
				short int result_of_insert;
				result_of_insert = insert(StaticList, index_to_insert, value_to_insert);
				if (result_of_insert == 1) {
					cout << "\nValue successfully inserted\n";
				}
				else if (result_of_insert == -1) {
					cout << "\nList if already full\n";
				}
				else {
					cout << "\nNo element with this index in list\n";
				}
			}
			break;
		}
		case 4: { // remove by index
			if (StaticList.array == nullptr) {
				cout << "\nList isn't created\n";
			}
			else {
				size_t index_to_remove = get_index();
				short int result_of_remove;
				result_of_remove = remove(StaticList, index_to_remove);
				if (result_of_remove == 1) {
					cout << "\nValue successfully removed\n";
				}
				else if (result_of_remove == 0) {
					cout << "\nNo element with this index in list\n";
				}
			}
			break;
		}
		case 5: { // get by index
			if (StaticList.array == nullptr) {
				cout << "\nList isn't created\n";
			}
			else {
				size_t index_to_search = get_index();
				Point* result_of_search = get(StaticList, index_to_search);
				if (result_of_search == nullptr) {
					cout << "\nNo element with this index in list\n";
				}
				else {
					cout << "\nYour point is: ";
					print_Point(*result_of_search);
					cout << endl;
				}
			}
			break;
		}
		case 6: { // length
			if (StaticList.array == nullptr) {
				cout << "\nList isn't created\n";
			}
			else {
				cout << "\nSize of list: " << length(StaticList) << endl;
			}
			break;
		}
		case 7: { // print list
			if (StaticList.array == nullptr) {
				cout << "\nList isn't created\n";
			}
			else {
				print_List(StaticList);
			}
			break;
		}
		default: break;
		}
	}

}

/*
template <typename ElementType>
StaticArrayList<ElementType> create_empty(const size_t SIZE) {
	ElementType arr[20];
	StaticArrayList<ElementType> new_list;
	new_list.array = arr;
	new_list.size = 0;
	new_list.capacity = SIZE;
	return new_list;
}
*/


template <typename ElementType>
short int append(StaticArrayList<ElementType>& list,  const ElementType value) {
	if (list.size == list.capacity) { return 0; }
	list.array[list.size] = value;
	list.size++;
	return 1;
}

template <typename ElementType>
short int insert(StaticArrayList<ElementType>& list, const size_t index_to_insert, const ElementType value) {
	if (list.size == list.capacity) { return -1; } // full
	if (index_to_insert >= list.size) { return 0; } // no index
	for (size_t i = index_to_insert + 1; i <= list.size; i++) {
		list.array[i] = list.array[i - 1];
	}
	list.array[index_to_insert] = value;
	list.size++;
	return 1;
}

template <typename ElementType>
short int remove(StaticArrayList<ElementType>& list, const size_t index_to_remove) {
	if (index_to_remove >= list.size) { return 0; } // no index
	for (size_t i = index_to_remove; i < list.size; i++) {
		list.array[i] = list.array[i + 1];
	}
	list.size--;
	return 1;
}

template <typename ElementType>
ElementType* get(StaticArrayList<ElementType>& list, const size_t index_to_search) {
	if (index_to_search >= list.size) { return nullptr; }
	else {
		return &list.array[index_to_search];
	}
}

template <typename ElementType>
size_t length(StaticArrayList<ElementType>& list) {
	return list.size;
}

template <typename ElementType>
void print_List(StaticArrayList<ElementType>& list) {
	if (list.size == 0) {
		cout << "\nList is empty\n";
		return;
	}
	cout << endl << "Your list is: \n";
	for (size_t i = 0; i < list.size; i++) {
		print_Point(list.array[i]);
	}
}

