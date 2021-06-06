#include <iostream>
#include <cassert>
#include <vector>


#include "Time.h"
#include "linked_list.h"
#include "ArrayList.h"
#include "BST.h"
#include "AvlTree.h"


int main() {
	/*
	BinTreeNode* list = create_bst();
	print(list);
	std::cout << std::endl;
	add(list, Time{ 2007, 05, 15, 20, 45, 23 });
	add(list, Time{ 2020, 05, 15, 20, 45, 23 });
	add(list, Time{ 2007, 05, 15, 20, 45, 23 });
	add(list, Time{ 2000, 05, 15, 20, 45, 23 });
	add(list, Time{ 1975, 05, 15, 20, 45, 23 });
	add(list, Time{ 2005, 05, 15, 20, 45, 23 });
	print(list);
	std::cout << "\n\n\nPrint as a tree\n\n";
	print_as_tree(list);
	
	
	Time time{ 1800, 05, 15, 20, 45, 23 };
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(list, time);
	print_as_tree(list);
	print(list);

	time.year = 2007;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(list, time);
	print_as_tree(list);
	print(list);
	

	time.year = 1975;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(list, time);
	print_as_tree(list);
	print(list);
	

	time.year = 2020;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(list, time);
	print_as_tree(list);
	print(list);

	time.year = 2005;
	std::cout << "\nremove " << time << std::endl << std::endl;
	remove(list, time);
	print_as_tree(list);
	print(list);


	std::cout << "\n\n\nAdd random items:\n\n";
	add_random_items(list, 10);
	print(list);

	std::cout << "\n\n\nPrint as a tree\n\n";
	print_as_tree(list);
	std::cout << "\n\nSEARCH:" << std::endl << std::endl;
	print_search_result(search(list, Time{ 2000, 05, 15, 20, 45, 23 }));


	std::cout << "\n\nSEARCH in interval:" << std::endl << std::endl;
	print_search_result(search(list, Time{ 1800, 05, 15, 20, 45, 23 }, Time{ 2000, 05, 15, 20, 45, 23 }));


	short int year_summa = 0;
	process_list(list, [&](Time& time) {std::cout << time.year << std::endl; year_summa += time.year; });
	std::cout << "\n\n\nSumma of years: " << year_summa << std::endl;

	

	std::cout << "\n\n\nMemory: " << get_memory(list) << std::endl;
	clear(list);
	print(list);
	std::cout << "\n\n\nMemory: " << get_memory(list) << std::endl;

	add(list, random_time());
	print(list);
	std::cout << "\n\n\nMemory: " << get_memory(list) << std::endl;
	*/


	/*
	std::cout << "\n\n\nARRAY LIST\n\n\n";

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

	*/

	std::cout << "\n\n\nAVL tree\n\n\n";
	AvlNode* list = create_avl();
	print(list);
	std::cout << std::endl;
	add(list, Time{ 2007, 05, 15, 20, 45, 23 });
	add(list, Time{ 2020, 05, 15, 20, 45, 23 });
	add(list, Time{ 2007, 05, 15, 20, 45, 23 });
	add(list, Time{ 2000, 05, 15, 20, 45, 23 });
	add(list, Time{ 2011, 05, 15, 20, 45, 23 });
	add(list, Time{ 2001, 05, 15, 20, 45, 23 });
	add(list, Time{ 1975, 05, 15, 20, 45, 23 });
	add(list, Time{ 2005, 05, 15, 20, 45, 23 });
	add(list, Time{ 2006, 05, 15, 20, 45, 23 });
	add(list, Time{ 2010, 05, 15, 20, 45, 23 });
	add(list, Time{ 2009, 05, 15, 20, 45, 23 });
	add(list, Time{ 2008, 05, 15, 20, 45, 23 });
	add(list, Time{ 2022, 05, 15, 20, 45, 23 });
	add(list, Time{ 2018, 05, 15, 20, 45, 23 });
	add(list, Time{ 2023, 05, 15, 20, 45, 23 });
	add(list, Time{ 2024, 05, 15, 20, 45, 23 });
	print(list);
	std::cout << "\n\nPrint as tree:\n";
	print_as_tree(list);

	short int year;
	std::cout << "\n\n\nRemoving:\n\n";

	year = 2011;
	std::cout << "\n1. Remove " << year << ":\n";
	remove(list, Time{ year, 05, 15, 20, 45, 23 });
	print(list);
	print_as_tree(list);

	year = 2013;
	std::cout << "\n2. Remove " << year << ":\n";
	remove(list, Time{ year, 05, 15, 20, 45, 23 });
	print(list);
	print_as_tree(list);

	year = 2024;
	std::cout << "\n3. Remove " << year << ":\n";
	remove(list, Time{ year, 05, 15, 20, 45, 23 });
	print(list);
	print_as_tree(list);

	year = 2011;
	std::cout << "\n4. Remove " << year << ":\n";
	remove(list, Time{ year, 05, 15, 20, 45, 23 });
	print(list);
	print_as_tree(list);

	year = 1975;
	std::cout << "\n5. Remove " << year << ":\n";
	remove(list, Time{ year, 05, 15, 20, 45, 23 });
	print(list);
	print_as_tree(list);

	year = 2007;
	std::cout << "\n6. Remove " << year << ":\n";
	remove(list, Time{ year, 05, 15, 20, 45, 23 });
	print(list);
	print_as_tree(list);

	year = 2006;
	std::cout << "\n7. Remove " << year << ":\n";
	remove(list, Time{ year, 05, 15, 20, 45, 23 });
	print(list);
	std::cout << std::endl;
	print_as_tree(list);





	std::system("pause");
	return 0;
}


