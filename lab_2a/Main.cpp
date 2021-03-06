#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>

//using namespace std;

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
short int insert(DoublyLinkedList<ElementType>* list, size_t index_to_insert, ElementType value);

template <typename ElementType>
short int remove(DoublyLinkedList<ElementType>* list, size_t index_to_remove);

template <typename ElementType>
short int remove_by_item(DoublyLinkedList<ElementType>* list, ListNode<ElementType>* node_to_remove);

template <typename ElementType>
ListNode<ElementType>* get(DoublyLinkedList<ElementType>* list, size_t index_to_search);

template <typename ElementType>
void set(DoublyLinkedList<ElementType>* list, ListNode<ElementType>* node_to_edit, ElementType new_data);

template <typename ElementType>
size_t length(DoublyLinkedList<ElementType>* list);

template <typename ElementType>
void Is_correct_value(ElementType& value_to_check, const int floor_value = -1, const int ceiling_value = -1);

Point get_point();

int get_index();
void print_Menu();

template <typename ElementType>
void print_List(DoublyLinkedList<ElementType>* list);

template <typename ElementType>
void print_List(std::vector<ElementType> array_list);

void print_Point(Point p);

void StaticArray_Realization();
void ListArray_Realization();

template <typename ElementType>
short int append(StaticArrayList<ElementType>& list, const ElementType value);

template <typename ElementType>
short int insert(StaticArrayList<ElementType>& list, const size_t index_to_insert, const ElementType value);

template <typename ElementType>
short int remove(StaticArrayList<ElementType>& list, const size_t index_to_remove);

template <typename ElementType>
ElementType* get(StaticArrayList<ElementType>& list, const size_t index_to_get);

template <typename ElementType>
void set(StaticArrayList<ElementType>& list, const size_t index_to_change, ElementType new_data);

template <typename ElementType>
size_t length(StaticArrayList<ElementType>& list);

template <typename ElementType>
void print_List(StaticArrayList<ElementType>& list);


template <typename ElementType>
short int remove_list(DoublyLinkedList<ElementType>*& list);

template <typename ElementType>
short int remove_list(StaticArrayList<ElementType>& list);

template <typename ElementType>
short int remove_list(std::vector<ElementType>& list);


template <typename ElementType>
long int get_memory(DoublyLinkedList<ElementType>* linked_list);

template <typename ElementType>
long int get_memory(StaticArrayList<ElementType> static_list);

template <typename ElementType>
long int get_memory(std::vector<ElementType> array_list);





int main() {
	
	DoublyLinkedList<Point>* MyList = nullptr;
	//bool is_list_created = false;
	const size_t SIZE = 10;
	std::cout << "Hello!\n\n";
	short int program_mode = 1, next = 1;
	while (program_mode) {
		std::cout << "\nChoose the mode\n" <<
			"0 - EXIT\n" <<
			"1 - Interactive mode\n" <<
			"2 - Demonstrative\n" <<
			"3 - Benchark\n\n";
		std::cin >> program_mode;
		Is_correct_value(program_mode, 0, 3);
		if (program_mode == Interactive) {
			std::cout << "\nChoose the realization of list (1 - linked list, 2 - static array, 3 - std::vector): ";
			short int realization;
			std::cin >> realization;
			Is_correct_value(realization, 1, 3);
			if (realization == 1) { // linked list
				while (true) {
					print_Menu();
					short int next;
					std::cin >> next;
					Is_correct_value(next, 0, 8);
					if (next == 0) {
						remove_list(MyList); break;
					}
					switch (next) {
					case 1: {
						if (MyList) {
							std::cout << "\nList is already created\n";
							break;
						}
						MyList = create_empty <Point>();
						//is_list_created = true;
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
							
							if (insert(MyList, index_to_insert, get_point()) == 1) {
								std::cout << "\nItem succesfully inserted\n";
							}
							else {
								std::cout << "\nNo element with this index, item wasn't inserted\n";
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
								std::cout << "\nChange this item? (0 - No, 1 - Yes)\n";
								bool next;
								std::cin >> next;
								Is_correct_value(next, 0, 1);
								if (next) {
									std::cout << "\nEnter new values\n";
									Point new_data = get_point();
									set(MyList, result_of_search, new_data);
								}
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
								length(MyList) << std::endl;
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

		// Linked list
		std::cout << std::endl << "\n\n===Linked list===\n\n";
		std::cout << "Created linked list and appended some elements\n";
		DoublyLinkedList<Point>* linked_list = create_empty<Point>();
		append(linked_list, { 0,0,0 });
		append(linked_list, { 0,0,1 });
		append(linked_list, { 1,2,3 });
		append(linked_list, { 1,4,5 });
		append(linked_list, { 2,6,10 });
		append(linked_list, { 123,100,0 });
		print_List(linked_list);
		std::cout << "\nInsert (45; 34; 56) before element with index 2\n";
		insert(linked_list, 2, { 45,34,56 });
		print_List(linked_list);
		std::cout << "\nRemove element with index 5\n";
		remove(linked_list, 5);
		print_List(linked_list);
		std::cout << "\nGet element with index 3 and change it on (55; 11; 100)\n";
		set(linked_list, get(linked_list, 3), {55,11,100});
		print_List(linked_list);
		std::cout << "\nLength of linked list: " << length(linked_list) << std::endl;
		std::cout << "\nRemove linked list\n";
		remove_list(linked_list);
		std::cout << "\nLinked list removed\n";

		// Static List
		std::cout << std::endl << "\n\n===Static array list===\n\n" << std::endl;
		std::cout << "Created static array list and appended some elements\n";
		StaticArrayList<Point> static_list;
		Point arr[SIZE];
		static_list.array = arr;
		static_list.size = 0;
		static_list.capacity = SIZE;
		append(static_list, { 0,0,0 });
		append(static_list, { 0,0,1 });
		append(static_list, { 1,2,3 });
		append(static_list, { 1,4,5 });
		append(static_list, { 2,6,10 });
		append(static_list, { 123,100,0 });
		print_List(static_list);
		std::cout << "\nInsert (45; 34; 56) before element with index 2\n";
		insert(static_list, 2, { 45,34,56 });
		print_List(static_list);
		std::cout << "\nRemove element with index 3\n";
		remove(static_list, 3);
		print_List(static_list);
		std::cout << "\nGet element with index 3 and change it on (55; 11; 100)\n";
		set(static_list, 3, { 55,11,100 });
		print_List(static_list);
		std::cout << "\nLength of static list: " << length(static_list) << std::endl;
		std::cout << "\nRemove static list\n";
		remove_list(static_list);
		std::cout << "\nStatic list removed\n";

		// Array list
		std::cout << std::endl << "\n\n===Array list (std::vector)===\n\n" << std::endl;
		std::cout << "Created Array list and appended some elements\n";
		std::vector<Point> array_list;
		array_list.reserve(5);
		array_list.push_back({ 0,0,0 });
		array_list.push_back({ 0,0,1 });
		array_list.push_back({ 1,2,3 });
		array_list.push_back({ 1,4,5 });
		array_list.push_back({ 2,6,10 });
		array_list.push_back({ 123,100,0 });
		print_List(array_list);
		std::cout << "\nInsert (45; 34; 56) before element with index 2\n";
		array_list.insert(array_list.begin() + 2, { 45,34,56 });
		print_List(array_list);
		std::cout << "\nRemove element with index 5\n";
		array_list.erase(array_list.begin() + 5);
		print_List(array_list);
		std::cout << "\nGet element with index 3 and change it on (55; 11; 100)\n";
		array_list[3] = { 55,11,100 };
		print_List(array_list);
		std::cout << "\nLength of array list: " << array_list.size() << std::endl;
		std::cout << "\nRemove array list\n";
		remove_list(array_list);
		std::cout << "\nArray list removed\n";


			break;
		}

		else if (program_mode == Benchmark) {
		long int begin_time = clock();
		long int start_time;
		unsigned int N = 10;
		size_t i = 0;
		const size_t SIZE = 81920;
		Point point_array[SIZE];
		DoublyLinkedList<Point>* linked_list;
		StaticArrayList<Point> static_list;
		std::vector<Point> array_list;
		std::ofstream file_result("result.txt");
		file_result << "Result of Benchmark mode.\nStatic list tested only up to 81920, because error, when try to create for more items\n";
		while (N <= 163840) { // 163 840
			file_result << "N = " << N << std::endl;
			std::cout << "N = " << N << std::endl;

			/* linked list */
			start_time = clock();
			linked_list = create_empty<Point>();
			long int create_time = (clock() - start_time);

			// append
			start_time = clock();
			for (i = 1; i <= 9 * N / 10; i++) {
				append(linked_list, { 5,5,5 });
			}
			long int append_time = (clock() - start_time);

			// insert
			start_time = clock();
			for (i = 1; i <= N / 10; i++) {
				insert(linked_list, N/5, { 0,0,0 });
			}
			long int insert_time = (clock() - start_time);
			long int Memory = get_memory(linked_list);

			// change
			start_time = clock();
			for (i = 1; i <= N / 5; i++) {
				set(linked_list, get(linked_list, i), { 100, 100, 100 });
			}
			long int get_set_time = (clock() - start_time);

			// remove item
			start_time = clock();
			for (i = 1; i <= N / 5; i++) {
				remove(linked_list, i);
			}
			long int remove_item_time = (clock() - start_time);

			// remove list
			start_time = clock();
			remove_list(linked_list);
			long int remove_list_time = (clock() - start_time);

			file_result << "Linked list\n" << "Create time\t\t" << create_time << " ms\n" <<
				"Append 0.9*N items\t" << append_time << " ms\n" << "Insert 0.1*N items\t" << insert_time <<
				" ms\n" << "Get-set 0.2*N items\t" << get_set_time << " ms\n" <<
				"Remove 0.2*N items\t" << remove_item_time << " ms\n" << "Remove list\t\t" << remove_list_time 
				<< " s\nMemory\t\t" << Memory << " bytes\n\n";

			
			if (N <= 81920) {
				/* Static list */
				// create
				start_time = clock();
				static_list.array = point_array;
				static_list.size = 0;
				static_list.capacity = SIZE;
				//Memory = get_memory(static_list);
				create_time = (clock() - start_time);

				//append
				start_time = clock();
				for (i = 1; i <= 9* N / 10; i++) {
					append(static_list, { 5,5,5 });
				}
				append_time = (clock() - start_time);

				//insert
				start_time = clock();
				for (i = 1; i <= N / 10; i++) {
					insert(static_list, N / 5, { 0,0,0 });
				}
				insert_time = (clock() - start_time);
				Memory = get_memory(static_list);
				//print_List(static_list);

				//change
				start_time = clock();
				for (i = 1; i <= N / 5; i++) {
					get(static_list, i);
					set(static_list, i, { 100, 100, 100 });
				}
				get_set_time = (clock() - start_time);

				//remove elements
				start_time = clock();
				for (i = 1; i <= N / 5; i++) {
					remove(static_list, i);
				}
				remove_item_time = (clock() - start_time);

				//remove list
				start_time = clock();
				remove_list(static_list);
				remove_list_time = (clock() - start_time);

				file_result << "Static list\n" << "Create time\t\t" << create_time << " ms\n" <<
					"Append 0.9*N items\t" << append_time << " ms\n" << "Insert 0.1*N items\t" << insert_time <<
					" ms\n" << "Get-set 0.2*N items\t" << get_set_time << " ms\n" <<
					"Remove 0.2*N items\t" << remove_item_time << " ms\n" << "Remove list\t\t" << remove_list_time 
					<< " s\nMemory\t\t" << Memory << " bytes\n\n";

			}


			/* array list (std::vector) */
			start_time = clock();
			array_list.reserve(N);
			create_time = (clock() - start_time);

			// create
			start_time = clock();
			for (i = 1; i <= 9* N / 10; i++) {
				array_list.push_back({ 5,5,5 });
			}
			append_time = (clock() - start_time);

			// insert
			start_time = clock();
			for (i = 1; i <= N / 10; i++) {
				array_list.insert(array_list.begin(), { 0,0,0 });
			}
			Memory = get_memory(array_list);
			//print_List(array_list);

			// change
			insert_time = (clock() - start_time);
			start_time = clock();
			for (i = 1; i <= N / 5; i++) {
				array_list[i] = { 100,100,100 };
			}
			get_set_time = (clock() - start_time);

			// remove item
			start_time = clock();
			for (i = 1; i <= N / 5; i++) {
				array_list.erase(array_list.begin() + i);
			}
			remove_item_time = (clock() - start_time);

			// remove list
			start_time = clock();
			remove_list(array_list);
			remove_list_time = (clock() - start_time);

			file_result << "Array list\n" << "Create time\t\t" << create_time << " ms\n" <<
				"Append 0.9*N items\t" << append_time << " ms\n" << "Insert 0.1*N items\t" << insert_time <<
				" ms\n" << "Get-set 0.2*N items\t" << get_set_time << " ms\n" <<
				"Remove 0.2*N items\t" << remove_item_time << " ms\n" << "Remove list\t\t" << remove_list_time
				<< " s\nMemory\t\t" << Memory << " bytes\n\n\n";

			N *= 2;
			std::cout << "Time: " << (float)(clock() - begin_time) / 1000 << 's' <<std::endl << std::endl;
		}
		file_result.close();


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
short int insert(DoublyLinkedList<ElementType>* list, size_t index_to_insert, ElementType value) {
	ListNode<ElementType>* new_node = new ListNode<ElementType>;
	ListNode<ElementType>* current_node = get(list, index_to_insert);
	if (current_node == nullptr) {
		return 0;
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
	return 1;
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
void set(DoublyLinkedList<ElementType>* list, ListNode<ElementType>* node_to_edit, ElementType new_data) {
	//std::cout << "\nEnter new values:\n";
	//node_to_edit->data = get_point();
	//std::cout << "\nData successfully changed!\n";
	node_to_edit->data = new_data;
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

Point get_point() {
	Point user_point;
	std::cout << "Enter a point coordinate X: ";
	std::cin >> user_point.x;
	Is_correct_value(user_point.x);

	std::cout << "Enter a point coordinate Y: ";
	std::cin >> user_point.y;
	Is_correct_value(user_point.y);

	std::cout << "Enter a point coordinate Z: ";
	std::cin >> user_point.z;
	Is_correct_value(user_point.z);

	return user_point;
}

int get_index() {
	int index;
	std::cout << "\nEnter index: ";
	std::cin >> index;
	Is_correct_value(index, 0);
	return index;
}

template <typename ElementType>
void print_List(DoublyLinkedList<ElementType>* list) {
	if (list->begin == nullptr) {
		std::cout << "\nList is empty\n";
		return;
	}
	int i = 0;
	ListNode<ElementType>* current_node = list->begin;
	std::cout << std::endl << "Your list is: \n";
	while (current_node) {
		std::cout << i << '\t';
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
		delete list;
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
		std::cin >> next;
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
					std::cout << std::endl;
					std::cout << "\nChange this item? (0 - No, 1 - Yes)\n";
					bool next;
					std::cin >> next;
					Is_correct_value(next, 0, 1);
					if (next) {
						std::cout << "\nEnter new values\n";
						Point new_data = get_point();
						set(StaticList, index_to_search, new_data);
					}
				}
			}
			break;
		}
		case 6: { // length
			if (StaticList.array == nullptr) {
				std::cout << "\nList isn't created\n";
			}
			else {
				std::cout << "\nSize of list: " << length(StaticList) <<
					"\nCapacity of list: " << StaticList.capacity << std::endl;
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
	for (size_t i = list.size; i >= index_to_insert + 1; i--) {
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
void set(StaticArrayList<ElementType>& list, const size_t index_to_change, ElementType new_data) {
	//std::cout << "\nEnter new values\n";
	list.array[index_to_change] = new_data;
	//std::cout << "\nItem successully changed\n";
	
}

template <typename ElementType>
size_t length(StaticArrayList<ElementType>& list) {
	return list.size;
}

template <typename ElementType>
void print_List(StaticArrayList<ElementType>& list) {
	if (list.size == 0) {
		std::cout << "\nList is empty\n";
		return;
	}
	std::cout << std::endl << "Your list is: \n";
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
		list.size = 0;
		return 1;
	}
}



void ListArray_Realization() {
	std::vector<Point> ArrayList;

	while (true) {
		print_Menu();
		short int next;
		std::cin >> next;
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
				std::cout << "\nItem successfully appended:\n";
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
					std::cout << "\nNo element with this index in ArrayList\n";
					break;
				}
				//std::cout << "\nEnter a point to insert:\n";
				Point item_to_insert = get_point();
				ArrayList.insert(ArrayList.begin() + index_to_insert, item_to_insert);
				std::cout << "\nItem successfully inserted in ArrayList\n";
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
					std::cout << "\nNo element with this index in ArrayList\n";
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
					std::cout << "\nNo element with this index in ArrayList\n";
					break;
				}
				std::cout << "\nYour point is: ";
				print_Point(ArrayList[index_to_search]);
				std::cout << "\nChange this item? (0 - No, 1 - Yes)\n";
				bool next;
				std::cin >> next;
				Is_correct_value(next, 0, 1);
				if (next) {
					std::cout << "\nEnter new values\n";
					ArrayList[index_to_search] = get_point();
					std::cout << "\nItem successully changed\n";
				}
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
				print_List(ArrayList);
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
short int remove_list(std::vector<ElementType>& list) {
	if (list.capacity() == 0) {
		return 0;
	}
	else {
		list.clear();
		list.shrink_to_fit();
		return 1;
	}
}

template <typename ElementType>
void print_List(std::vector<ElementType> array_list) {
	if (array_list.size() == 0) {
		std::cout << "\nArrayList is empty\n";
	}
	else {
		std::cout << "\nYour ArrayList is:\n";
		for (size_t i = 0; i < array_list.size(); i++) {
			std::cout << i << '\t';
			print_Point(array_list[i]);
		}
	}
}

template <typename ElementType>
long int get_memory(DoublyLinkedList<ElementType>* linked_list) {
	return (linked_list->size * sizeof(ListNode<ElementType>) + sizeof(*linked_list) + sizeof(*linked_list));
}

template <typename ElementType>
long int get_memory(StaticArrayList<ElementType> static_list) {
	return (static_list.size*sizeof(ElementType) + sizeof(StaticArrayList<ElementType>));
}

template <typename ElementType>
long int get_memory(std::vector<ElementType> array_list) {
	return (array_list.capacity()*sizeof(ElementType));
}

