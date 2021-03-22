#include <iostream>
#include<ctime>
#include<vector>
#include<algorithm>

struct Time {
	short int day;
	short int month;
	short int year;
	short int hours;
	short int minutes;
	short int seconds;
};

enum ProgramMode {
	Exit, Demonstrative, Benchmark
};

void merge_array(Time* arr, Time* result, std::size_t left, std::size_t middle, std::size_t right);
void merge_sort(Time*& arr, std::size_t size, short int mode = Benchmark);

// time_1 > time_2 = true
bool compare_time(Time time_1, Time time_2) {
	if (time_1.year > time_2.year) { return true; }
	else if (time_1.year == time_2.year) {
		if (time_1.month > time_2.month) { return true; }
		else if (time_1.month == time_2.month) {
			if (time_1.day > time_2.day) { return true; }
			else if (time_1.day == time_2.day) {
				if (time_1.hours > time_2.hours) { return true; }
				else if (time_1.hours == time_2.hours) {
					if (time_1.minutes > time_2.minutes) { return true; }
					else if (time_1.minutes == time_2.minutes) {
						if (time_1.seconds > time_2.seconds) { return true; }
						else if (time_1.seconds == time_2.seconds) {
							return false;
						}
					}
				}
			}
		}
	}

	return false;
}

// time_1 < time_2 = true
bool compare_time_stl(Time time_1, Time time_2) {
	if (time_1.year < time_2.year) { return true; }
	else if (time_1.year == time_2.year) {
		if (time_1.month < time_2.month) { return true; }
		else if (time_1.month == time_2.month) {
			if (time_1.day < time_2.day) { return true; }
			else if (time_1.day == time_2.day) {
				if (time_1.hours < time_2.hours) { return true; }
				else if (time_1.hours == time_2.hours) {
					if (time_1.minutes < time_2.minutes) { return true; }
					else if (time_1.minutes == time_2.minutes) {
						if (time_1.seconds < time_2.seconds) { return true; }
						else if (time_1.seconds == time_2.seconds) {
							return false;
						}
					}
				}
			}
		}
	}

	return false;
}

bool is_equal_time(Time time_1, Time time_2) {
	if (time_1.year != time_2.year) { return false; }
	else if (time_1.year == time_2.year) {
		if (time_1.month != time_2.month) { return false; }
		else if (time_1.month == time_2.month) {
			if (time_1.day != time_2.day) { return false; }
			else if (time_1.day == time_2.day) {
				if (time_1.hours != time_2.hours) { return false; }
				else if (time_1.hours == time_2.hours) {
					if (time_1.minutes != time_2.minutes) { return false; }
					else if (time_1.minutes == time_2.minutes) {
						if (time_1.seconds != time_2.seconds) { return false; }
						else if (time_1.seconds == time_2.seconds) {
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool is_sorted(Time* arr, std::size_t size) {
	for (std::size_t i = 0; i < size - 1; i++) {
		if (compare_time(arr[i], arr[i + 1]) == true) {
			return false;
		}
	}
	return true;
}

Time* generate_new_array(std::size_t size) {
	Time* arr = new Time[size];
	for (std::size_t i = 0; i < size; i++) {
		arr[i].hours = rand() % 23;
		arr[i].minutes = rand() % 59;
		arr[i].seconds = rand() % 59;
		arr[i].day = rand() % 30 + 1;
		arr[i].month = rand() % 11 + 1;
		arr[i].year = rand() % 15 + 2006;
	}
	//arr[size - 1] = arr[size - 2];
	return arr;
}

Time* generate_almost_sorted(std::size_t size, std::size_t probability) {
	if (probability == 0) {
		probability = 5;
	}
	Time* arr = generate_new_array(size);
	merge_sort(arr, size);
	for (std::size_t i = 0; i < size; i++) {
		if (i % probability == 0) {
			arr[i] = arr[size/2];
		}
	}
	return arr;
}

Time* generate_almost_equal(std::size_t size, std::size_t probability = 10) { // probability - is probability of not equal elements
	if (probability == 0) {
		probability = 10;
	}
	Time* arr = generate_new_array(size);
	for (std::size_t i = 0; i < size; i++) {
		if (i % probability != 0) {
			arr[i] = arr[0];
		}
	}
	return arr;
}

Time* generate_reverse_sorted(std::size_t size) { // probability - is probability of not equal element
	Time* arr = generate_new_array(size);
	merge_sort(arr, size);
	Time* result = new Time[size];
	for (std::size_t i = 0; i < size; i++) {
		result[i] = arr[size - i - 1];
	}
	delete[]arr;
	return result;
}


void print_array(Time* arr, std::size_t low, std::size_t high) {
	for (std::size_t i = low; i <= high; i++) {
		std::cout << arr[i].year << "-" << arr[i].month << "-" <<
			arr[i].day << "  " << arr[i].hours << ":" << arr[i].minutes << ":" <<
			arr[i].seconds << std::endl;
	}
}

void selection_sort(Time* arr, std::size_t low, std::size_t high, short int mode = Benchmark) {
	std::size_t min_index;
	if (low >= high) { return; }
	for (std::size_t i = low; i < high; i++) {
		min_index = i;
		for (std::size_t j = i + 1; j <= high; j++) {
			if (compare_time(arr[min_index], arr[j]) == true) {
				min_index = j;
			}
		}
		if (i != min_index) {
			if (mode == Demonstrative) {
				std::cout << "\nindex of current element = " << i << std::endl;
				std::cout << "min index = " << min_index << std::endl;
				print_array(arr, low, high);
			}
			
			std::swap(arr[i], arr[min_index]);
			
			
		}
	}
	if (mode == Demonstrative) {
		std::cout << "\nSorted array:\n";
		print_array(arr, low, high);
	}
}

std::size_t partition(Time* arr, std::size_t low, std::size_t high) {
	std::size_t pivot_index = (high + low)/2;
	//std::size_t pivot_index = high;
	Time pivot = arr[pivot_index];
	std::size_t i = low;
	for (std::size_t j = low; j <= high; j++) {
		if (compare_time(arr[j], pivot) == false) { //<=
			if (j == pivot_index) {
				pivot_index = i;
			}
			else if (i == pivot_index) {
				pivot_index = j;
			}
			std::swap(arr[j], arr[i]);
			i++;
		}
	}

	if (i != low) {
		i--;
	}
	
	if (i != pivot_index) {
		std::swap(arr[pivot_index], arr[i]);
		pivot_index = i;
	}

	return pivot_index;
}

void quick_sort(Time* arr, std::size_t low, std::size_t high, short int mode = Benchmark) {
	if (mode == Demonstrative) {
		//print_array(arr, low, high);
	}
	if (low < high) {
		std::size_t pi = partition(arr, low, high);
		if (mode == Demonstrative) {
			std::cout << "\npivot index = " << (pi - low) << std::endl;
			print_array(arr, low, high);
		
		}
		
		std::size_t left = pi;
		while (is_equal_time(arr[left], arr[pi]) == true) {
			left--;
		}
		left++;

		std::size_t right = pi;
		while (is_equal_time(arr[right], arr[pi]) == true) {
			right++;
		}
		if (right != 0) {
			right--;
		}
		

		if (left == 0) { 
			quick_sort(arr, right + 1, high, mode);
		}
		else {
			quick_sort(arr, low, left - 1, mode);
			quick_sort(arr, right + 1, high, mode);
		}
		
	}
}

void merge_sort(Time*& arr, std::size_t size, short int mode) {
	Time* extra_arr = new Time[size];
	std::size_t bucket_size = 1;
	std::size_t count = 0;
	Time* current_array = nullptr;
	
	while (bucket_size < size) {
		count++;
		//std::size_t i = 0;

		std::size_t left = 0;
		std::size_t middle = bucket_size;
		std::size_t right = 2 * bucket_size;

		while (true) {
			

			//left = (left < size) ? left : size;
			middle = (middle < size) ? middle : size;
			right = (right < size) ? right : size;

			

			if (count % 2 != 0) {
				current_array = extra_arr;
				merge_array(arr, extra_arr, left, middle, right);
			}
			else {
				current_array = arr;
				merge_array(extra_arr, arr, left, middle, right);
			}
			
			
			left += 2 * bucket_size;
			middle += 2 * bucket_size;
			right += 2 * bucket_size;

			if (left >= size) { break; }
		}
		
		if (mode == Demonstrative) {
			std::cout << "\n merging #" << count << std::endl;
			print_array(current_array, 0, size - 1);
		}
		


		bucket_size *= 2;
	}
	if (count % 2 != 0) {
		delete[]arr;
		arr = extra_arr;
		
	}
	else {
		delete[] extra_arr;
	}
}

void merge_array(Time* arr, Time*result, std::size_t left, std::size_t middle, std::size_t right) {
	std::size_t i = left, j = middle , k = left;

	while (i < middle && j < right) {

		if (compare_time(arr[i], arr[j]) == false) {
			result[k] = arr[i];
			i++;
		}
		else {
			result[k] = arr[j];
			j++;
		}
		k++;
	}

	while (i < middle) {
		result[k] = arr[i];
		i++;
		k++;
	}

	while (j < right) {
		result[k] = arr[j];
		j++;
		k++;
	}
	
}

void combine_sort(Time* arr, std::size_t low, std::size_t high, std::size_t border, short int mode = Benchmark) {
	if (low < high) {
		if (high - low <= border) {
			selection_sort(arr, low, high);
			if (mode == Demonstrative) {
				std::cout << "Border = " << border << ", so selection sort:\n\n";
				print_array(arr, low, high);
			}
		}
		else {
			std::size_t pi = partition(arr, low, high);
			if (mode == Demonstrative) {
				std::cout << "\npivot index = " << (pi - low) << std::endl;
				print_array(arr, low, high);

			}


			std::size_t left = pi;
			while (is_equal_time(arr[left], arr[pi]) == true) {
				left--;
			}
			left++;

			std::size_t right = pi;
			while (is_equal_time(arr[right], arr[pi]) == true) {
				right++;
			}
			if (right != 0) {
				right--;
			}


			if (left == 0) {
				quick_sort(arr, right + 1, high, mode);
			}
			else {
				quick_sort(arr, low, left - 1, mode);
				quick_sort(arr, right + 1, high, mode);
			}
		}
		

	}
}










int main() {
	short int mode = 0;
	std::cout << "\nHello, chose mode (0 - Exit, 1 - Demonstration, 2 - Benchmark)\n";
	std::cin >> mode;
	if (mode == Demonstrative) {
		std::size_t SIZE = 30;
		Time* array_1 = generate_new_array(SIZE);
		Time* array_2 = new Time[SIZE];
		Time* array_3 = new Time[SIZE];
		Time* array_4 = new Time[SIZE];
		Time* array_5 = new Time[SIZE];
		Time* array_6 = generate_almost_sorted(SIZE, 5);
		Time* array_7 = generate_almost_equal(SIZE);
		Time* array_8 = generate_reverse_sorted(SIZE);
		for (std::size_t i = 0; i < SIZE; i++) {
			array_5[i] = array_4[i] = array_3[i] = array_2[i] = array_1[i] = array_1[0];
		}

		std::cout << "\n1. Selection sort:\n";
		selection_sort(array_1, 0, SIZE - 1, Demonstrative);

		std::cout << "\n2. Quick sort:\n";
		quick_sort(array_2, 0, SIZE - 1, Demonstrative);
		std::cout << "\nSorted array:\n";
		print_array(array_2, 0, SIZE - 1);

		std::cout << "\n3. Merge sort:\n";
		merge_sort(array_3, SIZE, Demonstrative);

		std::cout << "\n4. Combo sort (quick + selection):\n";
		combine_sort(array_4, 0, SIZE - 1, 10, Demonstrative);
		std::cout << "\nSorted array:\n";
		print_array(array_4, 0, SIZE - 1);

		std::cout << "\n5. Quick sort (almost sorted elements):\n";
		quick_sort(array_5, 0, SIZE - 1, Demonstrative);
		std::cout << "\nSorted array:\n";
		print_array(array_5, 0, SIZE - 1);

		std::cout << "\n6. Quick sort (almost equal elements):\n";
		quick_sort(array_6, 0, SIZE - 1, Demonstrative);
		std::cout << "\nSorted array:\n";
		print_array(array_6, 0, SIZE - 1);

		std::cout << "\n7. Quick sort (reverse sorted elements):\n";
		quick_sort(array_7, 0, SIZE - 1, Demonstrative);
		std::cout << "\nSorted array:\n";
		print_array(array_7, 0, SIZE - 1);

		std::cout << "\n7. STL sort:\n";
		std::sort(array_8, array_8+SIZE, compare_time_stl);
		print_array(array_8, 0, SIZE - 1);
		


		delete[]array_1;
		delete[]array_2;
		delete[]array_3;
		delete[]array_4;
		delete[]array_5;
		delete[]array_6;
		delete[]array_7;
		delete[]array_8;
	}
	else if (mode == Benchmark) {
		const std::size_t SIZE = 1000000;
		Time* array_1 = generate_almost_sorted(SIZE, 5);
	//	print_array(array_1, 0, SIZE - 1);
		unsigned int start_time = clock();
		quick_sort(array_1, 0, SIZE - 1, Benchmark);
		std::cout << "Almost sorted array, quick sort, " << SIZE << " elements: " << (float)(clock() - start_time)/1000<< std::endl;
		std::cout << "\n\n\n\n\n\n\n";
	//	print_array(array_1, 0, SIZE - 1);

		if (is_sorted(array_1, SIZE) == true) {
			std::cout << "\n\n\n\n\nAlmost sorted array sort: YES\n\n\n";
		}
		else {
			std::cout << "\n\n\n\n\nAlmost sorted array sort: NO\n\n\n";
		}


		Time* array_2 = generate_almost_equal(SIZE, 5);
	//	print_array(array_2, 0, SIZE - 1);
		start_time = clock();
		quick_sort(array_2, 0, SIZE - 1, Benchmark);
		std::cout << "Almost equal array, quick sort, " << SIZE << " elements: " << (float)(clock() - start_time) / 1000 << std::endl;
		std::cout << "\n\n\n\n\n\n\n";
	//	print_array(array_2, 0, SIZE - 1);

		if (is_sorted(array_2, SIZE) == true) {
			std::cout << "\n\n\n\n\Almost equal array sort: YES\n\n\n";
		}
		else {
			std::cout << "\n\n\n\n\nAlmost equal array sort: NO\n\n\n";
		}


	}
	else {

	}
	std::cout << "\n\nEND!\n\n";




	























	/*

	const std::size_t SIZE = 4000;
	Time* array_1 = generate_new_array(SIZE);
	Time* array_2 = generate_new_array(SIZE);
	Time* array_3 = generate_new_array(SIZE);
//	print_array(array_2, SIZE);

	for (std::size_t i = 1; i < SIZE; i++) {
		array_1[i] = array_1[i - 1];
		array_2[i] = array_1[i];
		array_3[i] = array_1[i];
	}


	std::cout << "\n\n\n\n";
	unsigned int start_time = clock();
	merge_sort(array_1, SIZE);
	std::cout << std::endl << SIZE << " elements, merge sort =" << (float)(clock() - start_time) / 1000 << "\n\n";
//	print_array(array_1, SIZE);
//	delete[]array_1;


	std::cout << "\n\n\n\n";
	start_time = clock();
	quick_sort(array_2,0, SIZE-1);
	std::cout << std::endl << SIZE << " elements, quick sort =" << (float)(clock() - start_time) / 1000 << "\n\n";
//	print_array(array_2, SIZE);
//	delete[] array_2;


	std::cout << "\n\n\n\n";
	start_time = clock();
	combine_sort(array_3, 0, SIZE - 1, 100);
	std::cout << std::endl << SIZE << " elements, combine sort =" << (float)(clock() - start_time) / 1000 << "\n\n";
	//	print_array(array_3, SIZE);
	//	delete[] array_3;












	if (is_sorted(array_1, SIZE) == true) {
		std::cout << "\n\n\n\n\nmerge sort: YES\n\n\n";
	}
	else {
		std::cout << "\n\n\n\n\nmerge sort: NO\n\n\n";
	}

	if (is_sorted(array_2, SIZE) == true) {
		std::cout << "\n\n\n\n\nquick sort: YES\n\n\n";
	}
	else {
		std::cout << "\n\n\n\n\nquick sort: NO\n\n\n";
	}

	if (is_sorted(array_3, SIZE) == true) {
		std::cout << "\n\n\n\n\ncombine sort: YES\n\n\n";
	}
	else {
		std::cout << "\n\n\n\n\ncombine sort: NO\n\n\n";
	}
	
	*/
	std::system("pause");
	return 0;
}