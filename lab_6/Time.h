#pragma once

#ifndef TIME_H
#define TIME_H


#include <iostream>
#include <cassert>


struct Time {
	short int year;
	short int month;
	short int day;

	short int hours;
	short int minutes;
	short int seconds;

};


bool operator> (Time& a, Time& b) {
	if (a.year > b.year) { return true; }
	else if (a.year == b.year) {
		if (a.month > b.month) { return true; }
		else if (a.month == b.month) {
			if (a.day > b.day) { return true; }
			else if (a.day == b.day) {
				if (a.hours > b.hours) { return true; }
				else if (a.hours == b.hours) {
					if (a.minutes > b.minutes) { return true; }
					else if (a.minutes == b.minutes) {
						if (a.seconds > b.seconds) { return true; }
						else if (a.seconds == b.seconds) {
							return false;
						}
					}
				}
			}
		}
	}
	return false;
}




bool operator== (Time& a, Time& b) {
	if (a.year == b.year && a.month == b.month && a.day == b.day && a.hours == b.hours && a.minutes == b.minutes && a.seconds == b.seconds) { return true; }
	return false;
}

bool operator!= (Time& a, Time& b) {
	return !(a == b);
}

bool operator>= (Time& a, Time& b) {
	return (a > b || a == b);
}

bool operator< (Time& a, Time& b) {
	return !(a > b || a == b);
}

bool operator<= (Time& a, Time& b) {
	return (a < b || a == b);
}

std::ostream& operator<< (std::ostream& out, Time& time) {
	out << time.day << "." << time.month << "." << time.year << ", " <<
		time.hours << ":" << time.minutes << ":" << time.seconds;
	return out;
}

Time random_time() {
	Time new_time;
	new_time.year = rand() % 46 + 1975;
	new_time.month = rand() % 12 + 1;
	new_time.day = rand() % 30 + 1;
	new_time.hours = rand() % 24;
	new_time.minutes = rand() % 60;
	new_time.seconds = rand() % 60;

	return new_time;
}

Time get_time() {
	Time t;
	std::cout << "\nEnter a date dd/mm/yy: ";
	std::cin >> t.day >> t.month >> t.year;
	std::cout << "\nEnter a time hh/mm/ss: ";
	std::cin >> t.hours >> t.minutes >> t.seconds;

	return t;
}

template <typename T>
void print_search_result(T* result) {
	if (!result) { std::cout << "\nNot found\n"; }
	else {
		std::cout << "Search result: " << result->data << std::endl;
	}
}

template <typename T>
void print_search_result(std::vector<T*> result) {
	if (result.size() == 0) { std::cout << "\nNot found\n"; }
	else {
		std::cout << "Search result:\n";
		for (std::size_t i = 0; i < result.size(); i++) {
			std::cout << result[i]->data << std::endl;
		}
	}
}
#endif // TIME_H
