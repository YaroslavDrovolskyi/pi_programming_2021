#include <iostream>
#include <cassert>
#include <vector>

#include "Time.h"
#include "linked_list.h"
#include "ArrayList.h"
//#include "BST.h"


int main() {
	
	ListNode* list = create_linked_list();
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


	std::cout << "\n\n\nARRAY LIST\n\n\n";
	int* arr = new int[100];
	delete[]arr;
	ArrayList array_list = create_array_list();
	add(array_list, Time{ 2007, 05, 15, 20, 45, 22 });
	print(array_list);
	std::cout << "\n\n\n";
	add(array_list, Time{ 2020, 05, 15, 20, 45, 23 });
	print(array_list);
	std::cout << "\n\n\n";
	add(array_list, Time{ 2007, 05, 15, 20, 45, 23 });
	print(array_list);
	std::cout << "\n\n\n";
	add(array_list, Time{ 2000, 05, 15, 20, 45, 23 });
	print(array_list);
	std::cout << "\n\n\n";
	add(array_list, Time{ 1975, 05, 15, 20, 45, 23 });
	print(array_list);
	std::cout << "\n\n\n";
	add(array_list, Time{ 2005, 05, 15, 20, 45, 23 });
	print(array_list);

	
	Time time{ 2005, 05, 15, 20, 45, 23 };
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(array_list, time);
	print(array_list);

	time.year = 2007;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(array_list, time);
	print(array_list);

	time.year = 1975;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(array_list, time);
	print(array_list);

	time.year = 2020;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(array_list, time);
	print(array_list);

	time.year = 2000;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(array_list, time);
	print(array_list);

	time.year = 2007;
	time.seconds = 22;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(array_list, time);
	print(array_list);

	add_random_items(array_list, 100);
	std::cout << "\n\n\nCheck order: " << check_order(array_list) << std::endl;
	print(array_list);

	std::cout << "\n\nSEARCH:" << std::endl << std::endl;
	Time* result_2 = search(array_list, Time{ 1975, 05, 15, 20, 45, 22 });
	if (result_2) {
		std::cout << *result_2 << std::endl;
	}
	else {
		std::cout << "\nNot found\n";
	}


	std::cout << "\n\nSEARCH in interval:" << std::endl << std::endl;
	std::vector<Time*> result_v_2 = search(array_list, Time{ 1800, 05, 15, 20, 45, 23 }, Time{ 1990, 05, 15, 20, 45, 23 });
	if (result_v_2.size() > 0) {
		for (std::size_t i = 0; i < result_v_2.size(); i++) {
			std::cout << *result_v_2[i] << std::endl;
		}
	}
	else {
		std::cout << "\nNot found\n";
	}

	int summa = 0;
	process_list(array_list, [&](Time& time) {std::cout << time.year << std::endl; summa += time.year; });
	std::cout << "\n\n\nSumma of years: " << summa << std::endl;



	std::cout << "\n\n\nMemory: " << get_memory(array_list) << std::endl;
	clear(array_list);
	print(array_list);


	std::system("pause");
	return 0;
}


