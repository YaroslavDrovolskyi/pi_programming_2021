#include <iostream>
#include<ctime>

struct Time {
	short int day;
	short int month;
	short int year;
	short int hours;
	short int minutes;
	short int seconds;
};


void merge_array(Time* arr, Time* result, std::size_t left, std::size_t middle, std::size_t right);

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

void print_array(Time* arr, std::size_t size) {
	for (std::size_t i = 0; i < size; i++) {
		std::cout << arr[i].year << "-" << arr[i].month << "-" <<
			arr[i].day << "  " << arr[i].hours << ":" << arr[i].minutes << ":" <<
			arr[i].seconds << std::endl;
	}
}

void selection_sort(Time* arr, std::size_t size) {
	std::size_t min_index;
	for (std::size_t i = 0; i < size - 1; i++) {
		min_index = i;
		for (std::size_t j = i + 1; j < size; j++) {
			if (compare_time(arr[min_index], arr[j]) == true) {
				min_index = j;
			}
		}
		if (i != min_index) {
			std::swap(arr[i], arr[min_index]);
		}
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

void quick_sort(Time* arr, std::size_t low, std::size_t high) {
	if (low < high) {
		std::size_t pi = partition(arr, low, high);
		if (pi == 0) { return; }
		quick_sort(arr, low, pi - 1);
		quick_sort(arr, pi + 1, high);
	}
}

void merge_sort(Time*& arr, std::size_t size) {
	Time* extra_arr = new Time[size];
	std::size_t bucket_size = 1;
	std::size_t count = 0;
	
	while (bucket_size < size) {
		count++;
		//std::size_t i = 0;

		std::size_t left = 0;
		std::size_t middle = bucket_size;
		std::size_t right = 2 * bucket_size;


		while (true) {
			

			left = (left < size) ? left : size;
			middle = (middle < size) ? middle : size;
			right = (right < size) ? right : size;

			if (count % 2 != 0) {
				merge_array(arr, extra_arr, left, middle, right);
			}
			else {
				merge_array(extra_arr, arr, left, middle, right);
			}
			
			
			left += 2 * bucket_size;
			middle += 2 * bucket_size;
			right += 2 * bucket_size;

			if (left >= size) { break; }
		}
		
		bucket_size *= 2;
	}
	if (count % 2 != 0) {
		arr = extra_arr;
		delete[]arr;
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

int main() {
	const std::size_t SIZE = 200000;
	Time* array_1 = generate_new_array(SIZE);
	Time* array_2 = generate_new_array(SIZE);
//	print_array(array_2, SIZE);

	for (std::size_t i = 0; i < SIZE; i++) {
		array_2[i] = array_1[i];
	}


	std::cout << "\n\n\n\n";
	unsigned int start_time = clock();
	merge_sort(array_1, SIZE);
	std::cout << std::endl << SIZE << " elements, merge sort =" << (float)(clock() - start_time) / 1000 << "\n\n";
//	delete[]array_1;



	start_time = clock();
	quick_sort(array_2,0, SIZE-1);
	std::cout << std::endl << SIZE << " elements, quick sort =" << (float)(clock() - start_time) / 1000 << "\n\n";
//	print_array(array_2, SIZE);
//	delete[] array_2;

	/*
	std::cout << "\n\n\n\n";
	const std::size_t SIZE_2 = 9;
//	Time* arr_1 = generate_new_array(SIZE_2);
	Time* arr_2 = generate_new_array(SIZE_2);
	print_array(arr_2, SIZE_2);
	std::cout << "\n\n\n\n\n";
	//merge_array(arr_2, arr_1, 0, 1, 2);
	merge_sort(arr_2, SIZE_2);
	print_array(arr_2, SIZE_2);

	*/

	std::system("pause");
	return 0;
}