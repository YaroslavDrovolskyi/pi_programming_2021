#include <iostream>
#include <cassert>
#include <vector>


#include "Time.h"
#include "linked_list.h"
#include "ArrayList.h"
#include "BST.h"
#include "AvlTree.h"


void print_menu();
void interactive();
void demonstrative();

int main() {
	short int next;
	std::cout << "Hello!\n\n\n";
	std::cout << "Choose program mode: 0 - exit, 1 - interactive, 2 - demonstrative, 3 - benchamrk(NOT exist)\n\n";
	std::cin >> next;
	if (next == 1) {
		interactive();
	}
	else if (next == 2) {
		demonstrative();
	}
	else if (next == 3) {
//		benchmark();
	}
	std::cout << "\n\nTHE END!!!\n\n";
	std::system("pause");
	return 0;
}

void interactive() {
	while (true) {
		std::cout << "Chose realization: 0 - exit, 1 - Linked List, 2 - Array List, 3 - BST, 4 - AVL, 5 - 2-3tree (NOT exist)\n";
		short int realization = 0;
		std::cin >> realization;
		if (realization == 0) { return; }
		else if (realization == 1) { // linked list
			bool is_created = false;
			ListNode* list = nullptr;
			short int action = 1;
			while (action > 0) {
				print_menu();
				std::cin >> action;
				if (is_created == false && action != 1) {
					std::cout << "\nList isn't created\n";
					continue;
				}
				switch (action) {
					case 1: { // create empty
						if (is_created == false) {
							list = create_linked_list();
							is_created = true;
							std::cout << "\nLinked list successfully created!\n";
						}
						else {
							std::cout << "\nList is already created\n";
						}

						break;
					}
					case 2: { // add items
						Time new_item = get_time();
						add(list, new_item);
						std::cout << "\nSuccessfully added\n";
						break;
					}
					case 3: {
						int n = 0;
						std::cout << "Enter number of items (> 0): ";
						std:: cin >> n;
						assert(n > 0);
						add_random_items(list, n);
						std::cout << "\nSuccessfully added\n";
						break;
					}
					case 4: { // remove item
						Time new_item = get_time();
						remove(list, new_item);
						std::cout << "\nSuccessfully removed\n";
						break;
					}
					case 5: { // search
						std::cout << "\n0 - by value, 1 - in interval\n";
						bool by_val = true;
						std::cin >> by_val;
						if (by_val == true) {
							Time key = get_time();
							print_search_result(search(list, key));
						}
						else {
							std::cout << "\nStart of interval:\n";
							Time start = get_time();
							std::cout << "\nEnd of interval:\n";
							Time end = get_time();
							print_search_result(search(list, start, end));
						}
						break;
					}
					case 6: { // print
						print(list);
						break;	
					}	
					case 7: { // sum of years (process)
						if (list == nullptr) {
							std::cout << "\nList is empty\n";
						}
						else {
							int sum = 0;
							process_list(list, [&](Time& time) {sum += time.year; });
							std::cout << "Sum = " << sum << std::endl;
						}
						break;
					}
					case 8: { // get memory
						std::cout << "Using memory: " << get_memory(list) << " bytes\n";
						break;
					}
					case 9: { // clear
						clear(list);
						std::cout << "\nSuccessfully cleared\n";
						break;
					}
					default: break;
				}
			}
			clear(list);
		}
		else if (realization == 2) { // array list
			bool is_created = false;
			ArrayList list;
			short int action = 1;
			while (action > 0) {
				print_menu();
				std::cin >> action;
				if (is_created == false && action != 1) {
					std::cout << "\nList isn't created\n";
					continue;
				}
				switch (action) {
				case 1: { // create empty
					if (is_created == false) {
						list = create_array_list();
						is_created = true;
						std::cout << "\nLinked list successfully created!\n";
					}
					else {
						std::cout << "\nList is already created\n";
					}

					break;
				}
				case 2: { // add items
					Time new_item = get_time();
					add(list, new_item);
					std::cout << "\nSuccessfully added\n";
					break;
				}
				case 3: {
					int n = 0;
					std::cout << "Enter number of items (> 0): ";
					std::cin >> n;
					assert(n > 0);
					add_random_items(list, n);
					std::cout << "\nSuccessfully added\n";
					break;
				}
				case 4: { // remove item
					Time new_item = get_time();
					remove(list, new_item);
					std::cout << "\nSuccessfully removed\n";
					break;
				}
				case 5: { // search
					std::cout << "\n0 - by value, 1 - in interval\n";
					bool by_val = true;
					std::cin >> by_val;
					if (by_val == true) {
						Time key = get_time();
						Time* result = search(list, key);
						
						if (!result) { std::cout << "\nNot found\n"; }
						else {
							std::cout << "Search result: " << *result << std::endl;
						}

					}
					else {
						std::cout << "\nStart of interval:\n";
						Time start = get_time();
						std::cout << "\nEnd of interval:\n";
						Time end = get_time();
						std::vector<Time*>result = search(list, start, end);
						if (result.size() == 0) { std::cout << "\nNot found\n"; }
						else {
							std::cout << "Search result:\n";
							for (std::size_t i = 0; i < result.size(); i++) {
								std::cout << (*result[i]) << std::endl;
							}
						}
					}
					break;
				}
				case 6: { // print
					print(list);
					break;
				}
				case 7: { // sum of years (process)
					if (list.array == nullptr) {
						std::cout << "\nList is empty\n";
					}
					else {
						int sum = 0;
						process_list(list, [&](Time& time) {sum += time.year; });
						std::cout << "Sum = " << sum << std::endl;
					}
					break;
				}
				case 8: { // get memory
					std::cout << "Using memory: " << get_memory(list) << " bytes\n";
					break;
				}
				case 9: { // clear
					clear(list);
					std::cout << "\nSuccessfully cleared\n";
					break;
				}
				default: break;
				}
			}
			clear(list);
		}
		else if (realization == 3) { // bst
		bool is_created = false;
		BinTreeNode* bst = nullptr;
		short int action = 1;
		while (action > 0) {
			print_menu();
			std::cin >> action;
			if (is_created == false && action != 1) {
				std::cout << "\nList isn't created\n";
				continue;
			}
			switch (action) {
			case 1: { // create empty
				if (is_created == false) {
					bst = create_bst();
					is_created = true;
					std::cout << "\nLinked list successfully created!\n";
				}
				else {
					std::cout << "\nList is already created\n";
				}

				break;
			}
			case 2: { // add items
				Time new_item = get_time();
				add(bst, new_item);
				std::cout << "\nSuccessfully added\n";
				break;
			}
			case 3: {
				int n = 0;
				std::cout << "Enter number of items (> 0): ";
				std::cin >> n;
				assert(n > 0);
				add_random_items(bst, n);
				std::cout << "\nSuccessfully added\n";
				break;
			}
			case 4: { // remove item
				Time new_item = get_time();
				remove(bst, new_item);
				std::cout << "\nSuccessfully removed\n";
				break;
			}
			case 5: { // search
				std::cout << "\n0 - by value, 1 - in interval\n";
				bool by_val = true;
				std::cin >> by_val;
				if (by_val == true) {
					Time key = get_time();
					print_search_result(search(bst, key));
				}
				else {
					std::cout << "\nStart of interval:\n";
					Time start = get_time();
					std::cout << "\nEnd of interval:\n";
					Time end = get_time();
					print_search_result(search(bst, start, end));
				}
				break;
			}
			case 6: { // print
				print(bst);
				break;
			}
			case 7: { // sum of years (process)
				if (bst == nullptr) {
					std::cout << "\nList is empty\n";
				}
				else {
					int sum = 0;
					process_list(bst, [&](Time& time) {sum += time.year; });
					std::cout << "Sum = " << sum << std::endl;
				}
				break;
			}
			case 8: { // get memory
				std::cout << "Using memory: " << get_memory(bst) << " bytes\n";
				break;
			}
			case 9: { // clear
				clear(bst);
				std::cout << "\nSuccessfully cleared\n";
				break;
			}
			default: break;
			}
		}
		clear(bst);
		}
		else if (realization == 4) { // avl
		bool is_created = false;
		AvlNode* avl = nullptr;
		short int action = 1;
		while (action > 0) {
			print_menu();
			std::cin >> action;
			if (is_created == false && action != 1) {
				std::cout << "\nList isn't created\n";
				continue;
			}
			switch (action) {
			case 1: { // create empty
				if (is_created == false) {
					avl = create_avl();
					is_created = true;
					std::cout << "\nLinked list successfully created!\n";
				}
				else {
					std::cout << "\nList is already created\n";
				}

				break;
			}
			case 2: { // add items
				Time new_item = get_time();
				add(avl, new_item);
				std::cout << "\nSuccessfully added\n";
				break;
			}
			case 3: {
				int n = 0;
				std::cout << "Enter number of items (> 0): ";
				std::cin >> n;
				assert(n > 0);
				add_random_items(avl, n);
				std::cout << "\nSuccessfully added\n";
				break;
			}
			case 4: { // remove item
				Time new_item = get_time();
				remove(avl, new_item);
				std::cout << "\nSuccessfully removed\n";
				break;
			}
			case 5: { // search
				std::cout << "\n0 - by value, 1 - in interval\n";
				bool by_val = true;
				std::cin >> by_val;
				if (by_val == true) {
					Time key = get_time();
					print_search_result(search(avl, key));
				}
				else {
					std::cout << "\nStart of interval:\n";
					Time start = get_time();
					std::cout << "\nEnd of interval:\n";
					Time end = get_time();
					print_search_result(search(avl, start, end));
				}
				break;
			}
			case 6: { // print
				print(avl);
				break;
			}
			case 7: { // sum of years (process)
				if (avl == nullptr) {
					std::cout << "\nList is empty\n";
				}
				else {
					int sum = 0;
					process_list(avl, [&](Time& time) {sum += time.year; });
					std::cout << "Sum = " << sum << std::endl;
				}
				break;
			}
			case 8: { // get memory
				std::cout << "Using memory: " << get_memory(avl) << " bytes\n";
				break;
			}
			case 9: { // clear
				clear(avl);
				std::cout << "\nSuccessfully cleared\n";
				break;
			}
			default: break;
			}
		}
		clear(avl);
		}
	}
}	
					


void print_menu() {
	std::cout << "\n\n=======MENU=======\n" <<
		"1 \tcreate empty\n" <<
		"2 \tadd item\n" <<
		"3 \tadd random items\n" <<
		"4 \tremove item\n" <<
		"5 \tsearch by value\n" <<
		"6 \tprint\n" <<
		"7 \tcount sum of years (process)\n" <<
		"8 \tget memory\n" <<
		"9 \tclear\n" <<
		"==================\n\n";
}



void demonstrative() {
	{ // linked list
	std::cout << "\n\n\nLinked list\n\n";
	std::cout << "1. Create list and add 12 random items:\n";
	ListNode* linked_list = create_linked_list();
	add_random_items(linked_list, 10);
	Time time1{2020, 05, 30, 21, 55, 59}, time2{1991, 9, 1, 18, 35, 1};
	add(linked_list, time1);
	add(linked_list, time2);
	print(linked_list);
	
	std::cout << "\n\n2. Search:\n";
	std::cout << "\nSearch for " << time1 << "\n";
	print_search_result(search(linked_list, time1));

	std::cout << "\nSearch from " << time2 << " to " << time1 << "\n";
	print_search_result(search(linked_list, time2, time1));

	std::cout << "\n\n3. Sum of years (process):\n";
	int sum = 0;
	process_list(linked_list, [&](Time& time) {sum += time.year; });
	std::cout << "Sum = " << sum << std::endl;

	std::cout << "\n\n4. Using memory: " << get_memory(linked_list) << " bytes\n";

	std::cout << "\n\n5. Remove " << time2 << "\n";
	remove(linked_list, time2);
	print(linked_list);

	std::cout << "\n\n6. Clear list\n";
	clear(linked_list);
	print(linked_list);
	}

	{
	std::cout << "\n\n\nArray list\n\n";
	std::cout << "1. Create list and add 12 random items:\n";
	ArrayList array_list = create_array_list();
	add_random_items(array_list, 10);
	Time time1{ 2020, 05, 30, 21, 55, 59 }, time2{ 1991, 9, 1, 18, 35, 1 };
	add(array_list, time1);
	add(array_list, time2);
	print(array_list);

	std::cout << "\n\n2. Search:\n";
	std::cout << "\nSearch for " << time1 << "\n";
	Time* result = search(array_list, time1);

	if (!result) { std::cout << "\nNot found\n"; }
	else {
		std::cout << "Search result: " << *result << std::endl;
	}

	std::cout << "\nSearch from " << time2 << " to " << time1 << "\n";
	std::vector<Time*>result_v = search(array_list, time2, time1);
	if (result_v.size() == 0) { std::cout << "\nNot found\n"; }
	else {
		std::cout << "Search result:\n";
		for (std::size_t i = 0; i < result_v.size(); i++) {
			std::cout << (*result_v[i]) << std::endl;
		}
	}

	std::cout << "\n\n3. Sum of years (process):\n";
	int sum = 0;
	process_list(array_list, [&](Time& time) {sum += time.year; });
	std::cout << "Sum = " << sum << std::endl;

	std::cout << "\n\n4. Using memory: " << get_memory(array_list) << " bytes\n";

	std::cout << "\n\n5. Remove " << time2 << "\n";
	remove(array_list, time2);
	print(array_list);

	std::cout << "\n\n6. Clear list\n";
	clear(array_list);
	print(array_list);
	}

	{ // bst
		std::cout << "\n\n\nBST\n\n";
		std::cout << "1. Create list and add 12 random items:\n";
		BinTreeNode* bst = create_bst();
		add_random_items(bst, 10);
		Time time1{ 2020, 05, 30, 21, 55, 59 }, time2{ 1991, 9, 1, 18, 35, 1 };
		add(bst, time1);
		add(bst, time2);
		print(bst);

		std::cout << "\n\n2. Search:\n";
		std::cout << "\nSearch for " << time1 << "\n";
		print_search_result(search(bst, time1));

		std::cout << "\nSearch from " << time2 << " to " << time1 << "\n";
		print_search_result(search(bst, time2, time1));

		std::cout << "\n\n3. Sum of years (process):\n";
		int sum = 0;
		process_list(bst, [&](Time& time) {sum += time.year; });
		std::cout << "Sum = " << sum << std::endl;

		std::cout << "\n\n4. Using memory: " << get_memory(bst) << " bytes\n";

		std::cout << "\n\n5. Remove " << time2 << "\n";
		remove(bst, time2);
		print(bst);

		std::cout << "\n\n6. Clear list\n";
		clear(bst);
		print(bst);
	}

	{ // avl
		std::cout << "\n\n\AVL\n\n";
		std::cout << "1. Create list and add 12 random items:\n";
		AvlNode* avl = create_avl();
		add_random_items(avl, 10);
		Time time1{ 2020, 05, 30, 21, 55, 59 }, time2{ 1991, 9, 1, 18, 35, 1 };
		add(avl, time1);
		add(avl, time2);
		print(avl);

		std::cout << "\n\n2. Search:\n";
		std::cout << "\nSearch for " << time1 << "\n";
		print_search_result(search(avl, time1));

		std::cout << "\nSearch from " << time2 << " to " << time1 << "\n";
		print_search_result(search(avl, time2, time1));

		std::cout << "\n\n3. Sum of years (process):\n";
		int sum = 0;
		process_list(avl, [&](Time& time) {sum += time.year; });
		std::cout << "Sum = " << sum << std::endl;

		std::cout << "\n\n4. Using memory: " << get_memory(avl) << " bytes\n";

		std::cout << "\n\n5. Remove " << time2 << "\n";
		remove(avl, time2);
		print(avl);

		std::cout << "\n\n6. Clear list\n";
		clear(avl);
		print(avl);
	}
}