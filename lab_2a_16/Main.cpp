#include <iostream>
#include <string>

//using namespace std;

struct Time {
	short int hour, minutes, seconds;
};
struct Message {
	Time time;
	std::string message;
};

struct ListNode {
	ListNode* prev;
	ListNode* next;
	Message data;
};

struct MessageLog {
	ListNode* begin;
	ListNode* end;
	std::size_t size;
	std::size_t capacity;
};


int main() {




	std::system("pause");
	return 0;
}