#include <iostream>
#include <vector>

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
short int remove_by_item(DoublyLinkedList<ElementType>* list, ListNode<ElementType>* node_to_remove);


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
void ListArray_Realization();



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


template <typename ElementType>
short int remove_list(DoublyLinkedList<ElementType>*& list);

template <typename ElementType>
short int remove_list(StaticArrayList<ElementType>& list);

template <typename ElementType>
short int remove_list(vector<ElementType>& list);




int main() {
	
	DoublyLinkedList<Point>* MyList = nullptr;
	bool is_list_created = false;
	
	std::cout << "Hello!\n\n";
	short int program_mode = 1, next = 1;
	while (program_mode) {
		std::cout << "\nChoose the mode\n" <<
			"0 - EXIT\n" <<
			"1 - Interactive mode\n" <<
			"1 - Demonstrative\n" <<
			"3 - Benchark\n\n";
		cin >> program_mode;
		Is_correct_value(program_mode, 0, 3);
		if (program_mode == Interactive) {
			std::cout << "\nChoose the realization of list (1 -- linked list, 2 -- static array, 3 -- std::vector): ";
			short int realization;
			cin >> realization;
			Is_correct_value(realization, 1, 3);
			if (realization == 1) { // linked list
				while (true) {
					print_Menu();
					short int next;
					cin >> next;
					Is_correct_value(next, 0, 8);
					if (next == 0) {
						remove_list(MyList); break;
					}
					switch (next) {
					//case 0: {
					//	remove_list(MyList); break;
					//}
					case 1: {
						if (MyList) {
							std::cout << "\nList is already created\n";
							break;
						}
						MyList = create_empty <Point>();
						is_list_created = true;
						std::cout << "\nList is created\n";
						break;
					}
					case 2: { // add in the end
						if (!MyList) {
							std::cout << "List isn't created";
						}
						else {
							std::cout << "\nEnter item:\n";
							//Point new_point = get_point();
							append(MyList, get_point());
							std::cout << "\nItem added\n";
						}
						break;
					}
					case 3: { // insert by index
						if (!MyList) {
							std::cout << "\nList isn't created\n";
						}
						else {
							size_t index_to_insert = get_index();
							if (index_to_insert >= MyList->size) {
								std::cout << "\nNo element with this index\n";
							}
							else {
								if (insert(MyList, index_to_insert, get_point())) {
									std::cout << "\nItem succesfully inserted\n";
								}
								else {
									std::cout << "\nNo element with this index\n";
								}
							}
							
						}
						break;
					}
					case 4: { // remove by index
						if (!MyList) {
							std::cout << "\nList isn't created\n";
							break;
						}
						else {
							size_t index_to_remove = get_index();
							
							ListNode<Point>* item_to_remove = get(MyList, index_to_remove);
							if (!item_to_remove) {
								std::cout << "\nNo element with this index\n";
							}
							else {
								std::cout << "\nAre you sure to remove this element? (0 - No, 1 - Yes)\n";
								print_Point(item_to_remove->data);
								bool remove_or_not = false;
								std::cin >> remove_or_not;
								Is_correct_value(remove_or_not, 0, 1);
								if (!remove_or_not) {
									std::cout << "\nItem isn't removed\n";
									break;
								}
								else {
									remove_by_item(MyList, item_to_remove);
									std::cout << "\nItem successfully removed\n";
								}
							}
						}
						break;
					}
					case 5: { // get by index
						if (!MyList) {
							std::cout << "\nList isn't created\n";
						}
						else {
							int index_to_search = get_index();
							ListNode<Point>* result_of_search = get(MyList, index_to_search);
							if (result_of_search) {
								std::cout << "\nYour item is: ";
								print_Point(result_of_search->data);
							}
							else {
								std::cout << "\nNo element with this index\n";
							}
						
						}
						break;
					}
					case 6: { // length
						if (!MyList) {
							std::cout << "\nList isn't created\n";
						}
						else {
							std::cout << "Length of linked list is: " <<
								length(MyList) << endl;
						}
						break;
					}
					case 7: { // print list
						if (!MyList) {
							std::cout << "\nList isn't created\n";
						}
						else {
							print_List(MyList);
						}
						break;
					}
					case 8: {
						if (remove_list(MyList) == 0) {
							std::cout << "\nList isn't created\n";
						}
						else {
							std::cout << "\nLinked list successfully removed\n";
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
			ListArray_Realization();
			}
		}
		else if (program_mode == Demonstration) {
			break;
		}

		else if (program_mode == Benchmark) {
			break;
		}
	}
	std::cout << "\n\n\nTHE end!\n\n\n";
	std::system("pause");
	return 0;
}




template <typename ElementType>
short int remove(DoublyLinkedList<ElementType>* list, size_t index_to_remove) {
	ListNode<ElementType>* node_to_remove = get(list, index_to_remove);

	if (node_to_remove == nullptr) {
		return 0;
	}
	else {
		remove_by_item(list, node_to_remove);
		return 1;
	}
}


template <typename ElementType>
short int remove_by_item(DoublyLinkedList<ElementType>* list, ListNode<ElementType>* node_to_remove) {

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
	std::cout << "Enter a point coordinate X: ";
	cin >> user_point.x;
	Is_correct_value(user_point.x);

	std::cout << "Enter a point coordinate Y: ";
	cin >> user_point.y;
	Is_correct_value(user_point.y);

	std::cout << "Enter a point coordinate Z: ";
	cin >> user_point.z;
	Is_correct_value(user_point.z);

	return user_point;
}

int get_index() {
	int index;
	std::cout << "\nEnter index: ";
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
		cout << i << '\t';
		print_Point(current_node->data);
		current_node = current_node->next;
		i++;
	}
}

void print_Point(Point p) {
	std::cout << '(' << p.x << "; " << p.y << "; " << p.z << ")\n";
	return;
}

template <typename ElementType>
short int remove_list(DoublyLinkedList<ElementType>*& list) {
	if (!list) {
		return 0;
	}
	else {
		ListNode<ElementType>* current_node = list->begin;
		while (list->begin) {
			current_node = list->begin;
			list->begin = list->begin->next;
			delete current_node;
		}
		list = nullptr;
		return 1;
	}
	
}




void print_Menu() {
	std::cout << "\n\n=======MENU=======\n" <<
		"0 \tchoose mode\n" <<
		"1 \tcreate new list\n" <<
		"2 \tadd in the list end\n" <<
		"3 \tinsert in list by index\n" <<
		"4 \tremove element by index\n" <<
		"5 \tget element by index\n" <<
		"6 \tlength of list\n" <<
		"7 \tprint list\n" <<
		"8 \tremove list\n"
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
		Is_correct_value(next, 0, 8);
		switch (next) {
		case 0: { // only way to turn back
			//remove_list(StaticList);
			return;
			break;
		}
		case 1: { // create list
			if (!StaticList.array) {

				Point arr[SIZE];
				StaticList.array = arr;
				StaticList.size = 0;
				StaticList.capacity = SIZE;

				std::cout << "\nList successfully created\n";
			}
			else {
				std::cout << "\nList is already created\n";
			}
			break;
		}
		case 2: { // append
			if (StaticList.array == nullptr) {
				std::cout << "\nList isn't created\n";
			}
			else {
				if (append(StaticList, get_point()) == 1) {
					std::cout << "\nSucessfully appended\n";
				}
				else {
					std::cout << "\nError of append: List is already full\n";
				}
			}
			break;
		}
		case 3: { // insert by index
			if (StaticList.array == nullptr) {
				std::cout << "\nList isn't created\n";
			}
			else {
				size_t index_to_insert = get_index();
				if (index_to_insert >= StaticList.size) {
					std::cout << "\nNo element with this index\n";
				}
				else {
					Point value_to_insert = get_point();
					short int result_of_insert;
					result_of_insert = insert(StaticList, index_to_insert, value_to_insert);
					if (result_of_insert == 1) {
						std::cout << "\nValue successfully inserted\n";
					}
					else if (result_of_insert == -1) {
						std::cout << "\nList if already full\n";
					}
					else {
						std::cout << "\nNo element with this index in list\n";
					}
				}
				
			}
			break;
		}
		case 4: { // remove by index
			if (StaticList.array == nullptr) {
				std::cout << "\nList isn't created\n";
			}
			else {
				size_t index_to_remove = get_index();
				if (index_to_remove >= StaticList.size) {
					std::cout << "\nNo element with this index in StaticList\n";
				}
				else {
					std::cout << "\nAre you sure to revove this item? (0 - No. 1 - Yes)\n";
					print_Point(StaticList.array[index_to_remove]);
					bool remove_or_not = false;
					std::cin >> remove_or_not;
					Is_correct_value(remove_or_not, 0, 1);
					if (!remove_or_not) {
						std::cout << "\nItem wasn't remove\n";
					}
					else {
						remove(StaticList, index_to_remove);
						std::cout << "\nItem successfully removed\n";
					}
				}
			}
			break;
		}
		case 5: { // get by index
			if (StaticList.array == nullptr) {
				std::cout << "\nList isn't created\n";
			}
			else {
				size_t index_to_search = get_index();
				Point* result_of_search = get(StaticList, index_to_search);
				if (result_of_search == nullptr) {
					std::cout << "\nNo element with this index in list\n";
				}
				else {
					std::cout << "\nYour point is: ";
					print_Point(*result_of_search);
					std::cout << endl;
				}
			}
			break;
		}
		case 6: { // length
			if (StaticList.array == nullptr) {
				std::cout << "\nList isn't created\n";
			}
			else {
				std::cout << "\nSize of list: " << length(StaticList) << endl;
			}
			break;
		}
		case 7: { // print list
			if (StaticList.array == nullptr) {
				std::cout << "\nList isn't created\n";
			}
			else {
				print_List(StaticList);
			}
			break;
		}
		case 8: { // remove list
			if (StaticList.array == nullptr) {
				std::cout << "\nList isn't created\n";
			}
			else {
				remove_list(StaticList);
				std::cout << "\nList successfully removed\n";
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
		std::cout << i << '\t';
		print_Point(list.array[i]);
	}
}


template <typename ElementType>
short int remove_list(StaticArrayList<ElementType>& list) {
	if (!list.array) {
		return 0;
	}
	else {
		list.array = nullptr;
		return 1;
	}
}



void ListArray_Realization() {
	vector<Point> ArrayList;

	while (true) {
		print_Menu();
		short int next;
		cin >> next;
		Is_correct_value(next, 0, 8);
		switch (next) {
		case 0: { // only way to turn back
			remove_list(ArrayList);
			return;
			break;
		}
		case 1: { // create list
			if (ArrayList.capacity() != 0) {
				std::cout << "\nArrayList is already created\n";
			}
			else {
				ArrayList.reserve(5);
				std::cout << "\nArrayList successfully created\n";
			}
			
			break;
		}
		case 2: { // append
			if (ArrayList.capacity() == 0) {
				std::cout << "\nArrayList isn't created\n";
			}
			else {
				std::cout << "\nEnter a point to append:\n";
				Point item_to_append;
				item_to_append = get_point();
				ArrayList.push_back(item_to_append);
			}
			break;
		}
		case 3: { // insert by index
			if (ArrayList.capacity() == 0) {
				std::cout << "\nArrayList isn't created\n";
			}
			else {
				//std::cout << "\nEnter index to insert:\n";
				size_t index_to_insert = get_index();
				if (index_to_insert >= ArrayList.size()) {
					std::cout << "\nNo element with this index in ArryList\n";
					break;
				}
				//std::cout << "\nEnter a point to insert:\n";
				Point item_to_insert = get_point();
				ArrayList.insert(ArrayList.begin() + index_to_insert, item_to_insert);
				std::cout << "\nItem successfully inserted in ArryList\n";
			}
			break;
		}
		case 4: { // remove by index
			if (ArrayList.capacity() == 0) {
				std::cout << "\nArrayList isn't created\n";
			}
			else {
				//std::cout << "\nEnter index to remove:\n";
				size_t index_to_remove = get_index();
				if (index_to_remove >= ArrayList.size()) {
					std::cout << "\nNo element with this index in ArryList\n";
					break;
				}
				std::cout << "\nIt is your point. Are you sure to remove it?  (0 -- No, 1 -- Yes)\n";
				print_Point(ArrayList[index_to_remove]);
				bool remove_or_not = false;
				std::cin >> remove_or_not;
				Is_correct_value(remove_or_not, 0, 1);
				if (!remove_or_not) { 
					std::cout << "\nItem isn't removed\n";
					break; 
				}
				else {
					ArrayList.erase(ArrayList.begin() + index_to_remove);
					std::cout << "\nItem successfully removed\n";
				}
			}
			break;
		}
		case 5: { // get by index
			if (ArrayList.capacity() == 0) {
				std::cout << "\nArrayList isn't created\n";
			}
			else {
				size_t index_to_search = get_index();
				if (index_to_search >= ArrayList.size()) {
					std::cout << "\nNo element with this index in ArryList\n";
					break;
				}
				std::cout << "\nYour point is: ";
				print_Point(ArrayList[index_to_search]);
			}
			break;
		}
		case 6: { // length
			if (ArrayList.capacity() == 0) {
				std::cout << "\nArrayList isn't created\n";
			}
			else {
				std::cout << "\nArray list lenght: " << ArrayList.size();
			}
			break;
		}
		case 7: { // print list
			if (ArrayList.capacity() == 0) {
				std::cout << "\nArrayList isn't created\n";
			}
			else {
				if (ArrayList.size() == 0) {
					std::cout << "\nArrayList is empty\n";
				}
				else {
					std::cout << "Your ArrayList is:\n";
					for (size_t i = 0; i < ArrayList.size(); i++) {
						std::cout << i << '\t';
						print_Point(ArrayList[i]);
					}
				}
				
			}
			break;
		}
		case 8: { // remove list
			if (remove_list(ArrayList) == 1) {
				std::cout << "\nArrayList successfully removed\n";
			}
			else {
				std::cout << "\nArrayList isn't created\n";
			}
		}
		default: break;
		}
	}

}

template <typename ElementType>
short int remove_list(vector<ElementType>& list) {
	if (list.capacity() == 0) {
		return 0;
	}
	else {
		list.clear();
		list.shrink_to_fit();
		return 1;
	}
}
