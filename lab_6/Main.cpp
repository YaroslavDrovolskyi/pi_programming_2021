#include <iostream>
#include <cassert>
#include <vector>

#include "Time.h"
#include "linked_list.h"


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