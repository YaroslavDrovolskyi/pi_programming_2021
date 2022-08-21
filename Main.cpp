#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

struct date {
	short int day, month, year;
};
struct Time {
	short int hour, min, second;
};
enum MessageType {
	debug, info, warning, error, fatal
};


enum WorkMode {
	Turn_Back, Interactive, Demonstration, Benchmark
};

enum WriteMode {
	append_to_file, file_as_new
};

struct message {
	int id;
	string text;
	date date;
	Time time;
	int type; // debug, info, warning, error, fatal
	short int priority; // [0;200]
	float load_level; // [0;1]

};




short int WriteInBIN(vector<message>& Message_Log, string FileName, const size_t begin = 0, short int WriteMode = append_to_file);
short int WriteInTEXT(vector<message>& Message_Log, string FileName, const size_t begin = 0, short int WriteMode = append_to_file);

short int ReadFromBIN(string FileName, vector<message>& Message_Log);
short int ReadFromTEXT(string FileName, vector<message>& Message_Log);

short int RemoveElement(const int id_to_remove, vector<message>& Message_Log, const string TEXTfile, const string BINfile, short int program_mode = Interactive);
short int ChangeElement(const int id_to_change, vector<message>& Message_Log, const string TEXTfile, const string BINfile, short int program_mode = Interactive);

message GetFromUser();
void ReadFromUser(vector<message>& Message_Log);
void PrintDB(vector<message>& Message_Log);
void ClearDB(vector<message>& Message_Log, string BINfile, string TEXTfile);
short int ClearLocalStorage(vector<message>& local_storage);

void print(message Message);


vector <message> Search(vector<message>& Message_Log);
vector <message> AutoSearch(vector<message>& Message_Log, const short int search_mode, const int program_mode = Benchmark);
void PrintSearchResult(vector<message>Search_Result);

bool Compare(message Message, Time StartTime, Time FinishTime);
bool Compare(message Message, int MessageType, float LoadLevel);
bool Compare(message Message, string MessageStart);

Time Get_MessageTime();
bool CompareTime(Time time_1, Time time_2);
int Get_MessageType();
float Get_LoadLevel();

void Generate_newDB(vector<message>& Message_Log, const int SIZE);
string Generate_Line();

unsigned long int SizeOfFile(string filename);
unsigned long int size_of_local_storage(vector<message>& Message_Log);


template <typename ElementType>
void Is_correct_value(ElementType& value_to_check, const int floor_value = 0, const int ceiling_value = -1) {
	// ceiling value = -1 means that it is no ceil for value_to_check
	if (ceiling_value == -1) {
		while (value_to_check - floor_value < 0 || !cin.good()) {
			cout << "Enter valid value: \n";
			cin.clear();
			cin.ignore(256, '\n');
			cin >> value_to_check;
		}
		return;
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



int main() {
	
	cout << "Hello, dear user!\n Chose the mode, please:\n";
	string BINfile = "Messages log_BIN.txt";
	string TEXTfile = "Messages log_TEXT.txt";
	vector<message> Message_Log;
	short int Mode = -1, next = -1;
	while (true) {
		cout << "===== MENU =====\n" << 
			"0 \tEXIT\n" <<
			"1 \tInteractive mode\n" <<
			"2 \tDemonstration mode\n" <<
			"3 \tBenchmark mode\n" << 
			"================\n";
		cin >> Mode; Is_correct_value(Mode, 0, 3);
		if (Mode == Turn_Back) { 
			
			return 0;
		}
		else if (Mode == Interactive) {
			while (true) {
				cout << "\n===== MENU =====\n" <<
					"0 \tturn back\n" <<
					"1 \tfill the DB as new one\n" <<
					"2 \trestore DB from BIN file\n" <<
					"3 \trestore DB from TEXT file\n" <<
					"4 \tappend something to DB\n" <<
					"5 \tchange something in DB\n" <<
					"6 \tremove something from DB\n" <<
					"7 \tsearch\n" <<
					"8 \tprint DB\n" <<
					"9 \tremove all from local storage\n" <<
					"10 \tremove all DB\n" <<
					"================\n";
				cin >> next; Is_correct_value(next, 0, 10);

				if (next == 0) { break; }
				
				if (next == 1) {
					ClearDB(Message_Log, BINfile, TEXTfile);
					ReadFromUser(Message_Log);
					cout << "\nBD successfully created\n";
				}
				else if (next == 2) {
					if (Message_Log.size() != 0) {
						Message_Log.clear();
					}
					
					short int result = ReadFromBIN(BINfile, Message_Log);
					if (result == 1) {
						cout << "\nBD successfully restored from BIN file\n";
					}
					else if (result == 0) {
						cout << "\nBIN file is empty\n";
					}
					else {
						cout << "\nSomething is wrong with BIN file\n";
					}
				}
				else if (next == 3) {
					if (Message_Log.size() != 0) {
						Message_Log.clear();
					}

					short int result = ReadFromTEXT(TEXTfile, Message_Log);
					if (result == 1) {
						cout << "\nBD successfully restored from TEXT file\n";
					}
					else if (result == 0) {
						cout << "\nTEXT file is empty\n";
					}
					else {
						cout << "\nSomething is wrong with TEXT file\n";
					}
					
					
				}
				else if (next == 4) {
					ReadFromUser(Message_Log);
					cout << "\nSuccessfully added in DB\n";
				}
				else if (next == 5) {
					int id_to_change;
					if (Message_Log.size() == 0) {
						if (ReadFromTEXT("Messages log_TEXT.txt", Message_Log) != 1) {
							if (ReadFromBIN("Messages log_BIN.txt", Message_Log) != 1) {
								cout << "\nDB is empty\n";
								break;
							}
						}
					}
					cout << "Enter ID of data, that you want to change: ";
					cin >> id_to_change; Is_correct_value(id_to_change, 0);

					if (ChangeElement(id_to_change, Message_Log, TEXTfile, BINfile) == 1) {
						cout << "Data successfully changed\n";
					}
					else { cout << "No data with this ID in DB\n"; }
				}

				else if (next == 6) {
					int id_to_remove;
					if (Message_Log.size() == 0) {
						if (ReadFromTEXT("Messages log_TEXT.txt", Message_Log) != 1) {
							if (ReadFromBIN("Messages log_BIN.txt", Message_Log) != 1) {
								cout << "\nDB is empty\n";
								break;
							}
						}
					}
					cout << "Enter ID of data, that you want to remove: ";
					cin >> id_to_remove; Is_correct_value(id_to_remove, 0);
					
					short int result_of_removing;
					result_of_removing = RemoveElement(id_to_remove, Message_Log, TEXTfile, BINfile);
					if (result_of_removing == 1) {
						cout << "Data successfully removed\n";
					}
					else if (result_of_removing == 0){
						cout << "Data wasn't removed\n";
					}
					else if (result_of_removing == -1){
						cout << "No data with this ID in DB\n";
					}
					if (Message_Log.size() == 0) { cout << "\nDB is empty!\n"; }
				}

				else if (next == 7) {
					if (Message_Log.size() == 0) {
						if (ReadFromTEXT("Messages log_TEXT.txt", Message_Log) != 1) {
							if (ReadFromBIN("Messages log_BIN.txt", Message_Log) != 1) {
								cout << "\nDB is empty\n";
							}
						}
					}
					if (Message_Log.size() > 0) {
						PrintSearchResult(Search(Message_Log));
					}

				}
				else if (next == 8) {
					PrintDB(Message_Log);
				}
				else if (next == 9) {
					if (ClearLocalStorage(Message_Log) == 1) {
						cout << "\nLocal storage successfully cleared\n";
					}
					else {
						cout << "Local storage is already empty!\n";
					}
				}
				else if (next == 10) {
					ClearDB(Message_Log, BINfile, TEXTfile);
					cout << "\nDB successfully cleared\n";
				}
			}
		}
		else if (Mode == Demonstration) {
			//(0 -- debug, 1 -- info, 2 -- warning, 3 -- error, 4 -- fatal)

			cout << "1. Fill DB by fixed items\n\n\n";
			message demo_message_1{ 0, "Message #1", {10, 9, 1985},{21, 10, 18}, error, 8, 0.3 };
			message demo_message_2{ 1, "Message #2", {22, 10, 2001},{1, 15, 45}, debug, 45, 0.24 };
			message demo_message_3{ 2, "Message #3", {1, 2, 1976},{15, 48, 25}, warning, 165, 0.16 };
			message demo_message_4{ 3, "Message #4", {15, 6, 1989},{04, 35, 01}, info, 28, 0.199 };
			message demo_message_5{ 4, "Message #5", {31, 1, 2018},{8, 10, 16}, fatal, 14, 0.49 };
			message demo_message_6{ 5, "Message #6", {03, 4, 1919},{13, 51, 25}, warning, 183,  0.2345 };
			message demo_message_7{ 6, "Message #7", {01, 8, 1979},{06, 35, 14}, debug, 74,  0.99 };
			Message_Log.push_back(demo_message_1);
			Message_Log.push_back(demo_message_2);
			Message_Log.push_back(demo_message_3);
			Message_Log.push_back(demo_message_4);
			Message_Log.push_back(demo_message_5);
			WriteInTEXT(Message_Log, TEXTfile, 0, file_as_new);
			WriteInBIN(Message_Log, BINfile, 0, file_as_new);

			cout << "2. Delete data from local storage and restore from text file:\n\n";
			Message_Log.clear();
			ReadFromTEXT(TEXTfile, Message_Log);
			PrintDB(Message_Log);

			cout << "3. Delete data from local storage and restore from binary file:\n\n";
			Message_Log.clear();
			ReadFromBIN(BINfile, Message_Log);
			PrintDB(Message_Log);

			cout << "4. Append some other data\n\n";
			Message_Log.push_back(demo_message_6);
			Message_Log.push_back(demo_message_7);
			WriteInTEXT(Message_Log, TEXTfile, 5);
			WriteInBIN(Message_Log, BINfile, 5);

			cout << "5. Delete data from local storage and restore from binary file to check if adding was successful:\n\n\n";
			Message_Log.clear();
			ReadFromBIN(BINfile, Message_Log);
			PrintDB(Message_Log);

			cout << "6. Search data between 00:12:56 and 13:01:34 :\n\n\n";
			PrintSearchResult(AutoSearch(Message_Log, 1, Demonstration));
			cout << "\n\n\n=========\n\n\n\n";

			cout << "7. Search data with type \"info\" and load level >= 0.5:\n\n\n";
			PrintSearchResult(AutoSearch(Message_Log, 2, Demonstration));
			cout << "\n\n\n=========\n\n\n\n";

			cout << "8. Search data with message beginning \"Mess\":\n\n\n";
			PrintSearchResult(AutoSearch(Message_Log, 3, Demonstration));
			cout << "\n\n\n=========\n\n\n\n";

			cout << "9. Remove element with ID = 3\n\n\n";
			if (RemoveElement(3, Message_Log, TEXTfile, BINfile, Demonstration) == 1) {
				cout << "Data successfully removed\n";
			}
			else { cout << "No data with this ID in DB\n"; }
			PrintDB(Message_Log);


			cout << "10. Change message and priority of message with ID = 5\n";
			if (ChangeElement(5, Message_Log, TEXTfile, BINfile, Demonstration) == 1) {
				cout << "Data successfully changed\n";
			}
			else { cout << "No data with this ID in DB\n"; }
			PrintDB(Message_Log);


			cout << "11. Remove all from local storage:\n";
			if (ClearLocalStorage(Message_Log) == 1) {
				cout << "\nLocal storage successfully cleared\n";
			}
			else {
				cout << "Local storage is already empty!\n";
			}

			
			cout << "\n\n\n\n\nDemonstration mode end\n\n\n\n";

			break;
		}
		else if (Mode == Benchmark) {
			int START_TIME = clock();
			if (!Message_Log.empty()) { Message_Log.clear(); }
			int N = 10;
			int N_10 = N;

			unsigned int start_time, finish_time, gen_time_bin, gen_time_text, gen_time_vector,
				write_time_text, write_time_bin, read_time_text, read_time_bin, search_time_text, search_time_bin,
				search_time_vector;
			float TEXT_time, BIN_time;
			vector<message> Search_1, Search_2, Search_3;

			cout << "N = " << N;

			// to clear file for results
			ofstream file_for_results("results.txt");
			file_for_results << "Results of benchmark mode\n";
			file_for_results << "Testing all ways of saving until total time with binary files < 10s to deeper testing, because binary files is faster\n";
			file_for_results << "Time in columns: generation, write in file, read from file, search, total time\n" <<
				"For std::vector time of generation, search and total time\n\n\n";
			file_for_results.close();

			while (true) {
				Message_Log.reserve(N);
				// for text files
				start_time = clock();
				Generate_newDB(Message_Log, N);
				gen_time_text = clock() - start_time;

				start_time = clock();
				WriteInTEXT(Message_Log, "Benchmark_TEXT.txt", 0, file_as_new);
				write_time_text = clock() - start_time;

				Message_Log.clear();

				start_time = clock();
				ReadFromTEXT("Benchmark_TEXT.txt", Message_Log);
				read_time_text = clock() - start_time;

				start_time = clock();
				Search_1 = AutoSearch(Message_Log, 1);
				Search_2 = AutoSearch(Message_Log, 2);
				Search_3 = AutoSearch(Message_Log, 3);
				search_time_text = clock() - start_time;

				Message_Log.clear();
				Search_1.clear();
				Search_2.clear();
				Search_3.clear();


				TEXT_time = (float)(gen_time_text + write_time_text + read_time_text + search_time_text) / 1000;


				//for binary files
				start_time = clock();
				Generate_newDB(Message_Log, N);
				gen_time_bin = clock() - start_time;

				start_time = clock();
				WriteInBIN(Message_Log, "Benchmark_BIN.txt", 0, file_as_new);
				write_time_bin = clock() - start_time;

				Message_Log.clear();

				start_time = clock();
				ReadFromBIN("Benchmark_BIN.txt", Message_Log);
				read_time_bin = clock() - start_time;

				start_time = clock();
				Search_1 = AutoSearch(Message_Log, 1);
				Search_2 = AutoSearch(Message_Log, 2);
				Search_3 = AutoSearch(Message_Log, 3);
				search_time_bin = clock() - start_time;

				Message_Log.clear();
				Search_1.clear();
				Search_2.clear();
				Search_3.clear();

				BIN_time = (float)(gen_time_bin + write_time_bin + read_time_bin + search_time_bin) / 1000;

				// for std::vector
				start_time = clock();
				Generate_newDB(Message_Log, N);
				gen_time_vector = clock() - start_time;

				start_time = clock();
				Search_1 = AutoSearch(Message_Log, 1);
				Search_2 = AutoSearch(Message_Log, 2);
				Search_3 = AutoSearch(Message_Log, 3);
				search_time_vector = clock() - start_time;

				long int vector_memory = size_of_local_storage(Message_Log);
				Message_Log.clear();
				Search_1.clear();
				Search_2.clear();
				Search_3.clear();


				// write results in file
				ofstream file_for_results("results.txt", ofstream::app);
				file_for_results << "N = " << N << '\n';

				file_for_results << "TEXTfile: \t" << (float)gen_time_text / 1000 << "s\t" <<
					(float)write_time_text / 1000 << "s\t" << (float)read_time_text / 1000 << "s\t"
					<< (float)search_time_text / 1000 << "s\t" << TEXT_time << "s\t" << SizeOfFile("Benchmark_TEXT.txt") << " Bytes\n";

				file_for_results << "BINfile: \t" << (float)gen_time_bin / 1000 << "s\t" <<
					(float)write_time_bin / 1000 << "s\t" << (float)read_time_bin / 1000 << "s\t"
					<< (float)search_time_bin / 1000 << "s\t" << BIN_time << "s\t" << SizeOfFile("Benchmark_BIN.txt") << " Bytes\n";

				file_for_results << "VECTOR: \t" << (float)gen_time_vector / 1000 << "s\t" <<
					(float)search_time_vector / 1000 << "s\t" << (float)(gen_time_vector + search_time_vector) / 1000 << "s\t\t\t" << vector_memory << " Bytes\n\n\n";

				file_for_results.close();

				// print total time at the moment in console
				cout << "\ntime: " << (float)(clock() - START_TIME) / 1000 << "\n\n\n";

				// increase N
				if (BIN_time - 1 <= 0) {
					N *= 2;
					N_10 = N;
					cout << "N = " << N;
				}
				else if (BIN_time - 1 > 0 && BIN_time - 10 <= 0) {
					N += N_10;
					cout << "N = " << N;
				}
				else {
					break;
				}

			}
			cout << endl << "Benchmark mode ended\n\n\n";
			break;

		}

		else { cout << "Something is wrong"; return -1; }
	}

	system("pause");
	return 0;
}

short int WriteInBIN (vector<message>& Message_Log, string FileName, const size_t begin, short int WriteMode) {
	ofstream OutputFile;
	if (WriteMode == append_to_file) { OutputFile.open(FileName, ofstream::app | ios::binary); }
	else if (WriteMode == file_as_new) { OutputFile.open(FileName, ios::binary | ofstream::trunc); }

	if (!OutputFile.is_open()) {
		return -1;
	}
	for (size_t i = begin; i < Message_Log.size(); i++) {
		OutputFile.write((char*)&(Message_Log[i].id), sizeof(int));
		
		size_t size = (Message_Log[i].text).size();
		OutputFile.write((char*)&size, sizeof(size));
		OutputFile.write(&((Message_Log[i]).text)[0], size);  //text

		OutputFile.write((char*)&(Message_Log[i].date), sizeof(date));
		OutputFile.write((char*)&(Message_Log[i].time), sizeof(Time));
		OutputFile.write((char*)&(Message_Log[i].type), sizeof(MessageType));
		OutputFile.write((char*)&(Message_Log[i].priority), sizeof(short int));
		OutputFile.write((char*)&(Message_Log[i].load_level), sizeof(float));
	}
	OutputFile.close();
	return 1;
}



short int ReadFromBIN(string FileName, vector<message>& Message_Log) {
	ifstream InputFile;
	InputFile.open(FileName, ifstream::in | ios::binary);
	if (!InputFile.is_open()) {
		return -1;
	}
	if (InputFile.peek() == EOF) { return 0; }
	message Message;
	size_t size;
	while (InputFile.read((char*)&Message.id, sizeof(int))) {

		InputFile.read((char*)&size, sizeof(size));
		(Message.text).resize(size);
		InputFile.read(&(Message.text)[0], size);

		InputFile.read((char*)&Message.date, sizeof(date));
		InputFile.read((char*)&Message.time, sizeof(Time));
		InputFile.read((char*)&Message.type, sizeof(MessageType));
		InputFile.read((char*)&Message.priority, sizeof(short int));
		InputFile.read((char*)&Message.load_level, sizeof(float));
		Message_Log.push_back(Message);
	}
	InputFile.close();
	return 1;
}

short int WriteInTEXT(vector<message>& Message_Log, string FileName, const size_t begin, short int WriteMode) {
	ofstream File;
	if (WriteMode == append_to_file){ File.open(FileName, ofstream::app); }
	else if (WriteMode == file_as_new){ File.open(FileName); }
	
	if (!File.is_open()) {
		return -1;
	}
	for (size_t i = begin; i < Message_Log.size(); i++) {
		File << '\n' << Message_Log[i].id << '\t';
		File << (Message_Log[i].date).day << ' ';
		File << (Message_Log[i].date).month << ' ';
		File << (Message_Log[i].date).year << '\t';
		File << Message_Log[i].time.hour << ' ';
		File << Message_Log[i].time.min << ' ';
		File << Message_Log[i].time.second << '\t';
		File << Message_Log[i].type << '\t';
		File << Message_Log[i].priority << '\t';
		File << Message_Log[i].load_level << '\n';
		File << Message_Log[i].text;
	}
	File.close();
	return 1;
}



short int ReadFromTEXT(string FileName, vector<message>& Message_Log) {
	ifstream File;
	File.open(FileName);
	if (!File.is_open()) {
		return -1;
	}
	if (File.peek() == EOF) { return 0; }
	
	while (!File.eof()) {
		message Message;

		File >> Message.id;
		File >> Message.date.day;
		File >> Message.date.month;
		File >> Message.date.year;
		File >> Message.time.hour;
		File >> Message.time.min;
		File >> Message.time.second;
		File >> Message.type;
		File >> Message.priority;
		File >> Message.load_level;
		File.ignore(256,'\n');
		getline(File, Message.text);
	
		Message_Log.push_back(Message);
	}
	
	File.close();
	return 1;
	
}

short int RemoveElement(const int id_to_remove, vector<message>& Message_Log, const string TEXTfile, const string BINfile, short int program_mode) {
	int index_to_remove = -1;
	for (size_t i = 0; i < Message_Log.size(); i++) {
		if (id_to_remove == Message_Log[i].id) {
			index_to_remove = i;
			break;
		}
	}
	if (index_to_remove == -1) { return -1; }
	

	if (program_mode == Interactive) {
		cout << "It is this message:\n\n";
		print(Message_Log[index_to_remove]);
		cout << "\n\nAre you sure to remove this?   (0 - no, 1 - yes)\n\n";
		short int next;
		cin >> next; Is_correct_value(next, 0, 1);
		if (next == 0) { return 0; } // if no
	}
	
	Message_Log.erase(Message_Log.begin() + index_to_remove);
	WriteInBIN(Message_Log, BINfile, 0, file_as_new);
	WriteInTEXT(Message_Log, TEXTfile, 0, file_as_new);
	return 1;
}

short int ChangeElement(const int id_to_change, vector<message>& Message_Log, const string TEXTfile, const string BINfile, short int program_mode) {
	int index_to_change = -1;
	for (size_t i = 0; i < Message_Log.size(); i++) {
		if (id_to_change == Message_Log[i].id) {
			index_to_change = i;
			break;
		}
	}
	if (index_to_change == -1) { return 0; }
	if (program_mode == Interactive){ 
		cout << "It is this message:\n\n";
		print(Message_Log[index_to_change]);
		cout << "\nEnter a new values:\n\n";
		Message_Log[index_to_change] = GetFromUser();
		Message_Log[index_to_change].id = id_to_change;
	}
	else if (program_mode == Demonstration) {
		Message_Log[index_to_change].text = "Changed message";
		Message_Log[index_to_change].priority = 200;
	}
	
	WriteInBIN(Message_Log, BINfile, 0, file_as_new);
	WriteInTEXT(Message_Log, TEXTfile, 0, file_as_new);
	return 1;
}





message GetFromUser() {
	message new_message;
	cin.ignore(256,'\n');

	cout << "Enter a Message:  ";
	getline(cin, new_message.text);
	

	//cout << "Enter a date in format DD MM YY:  ";
	cout << "Enter a day:  ";
	cin >> new_message.date.day; Is_correct_value(new_message.date.day, 1, 31);

	cout << "Enter a month:  ";
	cin >> new_message.date.month; Is_correct_value(new_message.date.month, 1, 12);

	cout << "Enter a year:  ";
	cin >> new_message.date.year; Is_correct_value(new_message.date.year, 1);

	cout << "Enter a time hours:  ";
	cin >> new_message.time.hour; Is_correct_value(new_message.time.hour, 0, 23);

	cout << "Enter a time minutes:  ";
	cin >> new_message.time.min; Is_correct_value(new_message.time.min, 0, 59);

	cout << "Enter a time seconds:  ";
	cin >> new_message.time.second; Is_correct_value(new_message.time.second, 0, 59);

	cout << "Enter a Message Type (0 -- debug, 1 -- info, 2 -- warning, 3 -- error, 4 -- fatal):  ";
	cin >> new_message.type; Is_correct_value(new_message.type, 0, 4);

	cout << "Enter a Message priotity (int [0,200]):  ";
	cin >> new_message.priority; Is_correct_value(new_message.priority, 0, 200);

	cout << "Enter a Load level (float [0,1]):  ";
	cin >> new_message.load_level; Is_correct_value(new_message.load_level, 0, 1);

	return new_message;
}

void ReadFromUser(vector<message>& Message_Log) {
	// doesn't matter, if it will be nothing in text file
	if (Message_Log.size() == 0) { ReadFromTEXT("Messages log_TEXT.txt", Message_Log); }
	message Message;
	size_t begin = Message_Log.size();
	short int next = 1;
	while (next == 1) {
		Message = GetFromUser();
		//Giving ID to each element
		if (Message_Log.size() == 0) { Message.id = 0; }
		else {Message.id = Message_Log[Message_Log.size() - 1].id + 1;}
		Message_Log.push_back(Message);
		
		cout << "0 -- finish, 1 -- continue\n";
		cin >> next; Is_correct_value(next, 0, 1);
	}

	// Write added info in files
	WriteInTEXT(Message_Log, "Messages log_TEXT.txt", begin);
	WriteInBIN(Message_Log, "Messages log_BIN.txt", begin);
}

void print(message Message) {
	cout << "\nID: " << Message.id << endl;
	cout << "Text: " << Message.text << endl;
	cout << "Time: " << Message.time.hour << ' ' << Message.time.min << ' ' << Message.time.second << endl;
	cout << "Date: " << Message.date.day << ' ' << Message.date.month << ' ' << Message.date.year << endl;
	cout << "Type: ";
	//(0 -- debug, 1 -- info, 2 -- warning, 3 -- error, 4 -- fatal)
	if (Message.type == 0) { cout << "debug\n"; }
	else if (Message.type == 1) { cout << "info\n"; }
	else if (Message.type == 2) { cout << "warning\n"; }
	else if (Message.type == 3) { cout << "error\n"; }
	else if (Message.type == 4) { cout << "fatal\n"; }
	cout << "Priority: " << Message.priority << endl;
	cout << "Load level: " << Message.load_level << endl;
	cout << "\n============================\n";
}

void PrintDB(vector<message>& Message_Log) {
	if (Message_Log.size() == 0) {
		if (ReadFromTEXT("Messages log_TEXT.txt", Message_Log) != 1) {
			if (ReadFromBIN("Messages log_BIN.txt", Message_Log) != 1) {
				cout << "Nothing in local storage, nothing in TEXT and BIN files (DB is empty) or something with them\n";
				return;
			}
			else {
				cout << "Nothing in local storage, TEXT, printed from BINfile\n";
			}
		}
		else {
			cout << "Nothing in local storage, printed from TEXTfile\n";
		}
	}
	for (size_t i = 0; i < Message_Log.size(); i++) {
		print(Message_Log[i]);
	}
}
void ClearDB(vector<message>& Message_Log, string BINfile, string TEXTfile) {
	ofstream FileBIN, FileTEXT;
	Message_Log.clear();
	FileBIN.open(BINfile, ofstream::out | ofstream::trunc);
	FileTEXT.open(TEXTfile, ofstream::out | ofstream::trunc);
	FileBIN.close();
	FileTEXT.close();
}

short int ClearLocalStorage(vector<message>& local_storage) {
	if (local_storage.size() != 0) {
		local_storage.clear();
		return 1;
	}
	return 0;
}

vector <message> Search(vector<message>& Message_Log) {
	vector <message> Message_Result;
	short int search_mode;
	cout << "\nChoose criterias to search:\n" <<
		"1 - Time interval\n" <<
		"2 - Type and load level\n" <<
		"3 - Beginning of message\n";

	cin >> search_mode; Is_correct_value(search_mode, 1, 3);

	if (search_mode == 1) {
		Time StartTime, FinishTime;
		cout << "\nEnter the beginning time:\n";
		StartTime = Get_MessageTime();
		cout << "\nEnter the ending time:\n";
		FinishTime = Get_MessageTime();
		while (CompareTime(StartTime, FinishTime) == false) {
			cout << "\nEnter valid time (Begin time < Ending time):\n";
			cout << "\nEnter the beginning time:\n";
			StartTime = Get_MessageTime();
			cout << "\nEnter the ending time:\n";
			FinishTime = Get_MessageTime();
		}

		for (size_t i = 0; i < Message_Log.size(); i++) {
			if (Compare(Message_Log[i], StartTime, FinishTime) == true) {
				Message_Result.push_back(Message_Log[i]);
			}
		}
	}
	else if (search_mode == 2) {
		int MessageType;
		float LoadLevel;
		MessageType = Get_MessageType();
		LoadLevel = Get_LoadLevel();

		for (size_t i = 0; i < Message_Log.size(); i++) {
			if (Compare(Message_Log[i], MessageType, LoadLevel) == true) {
				Message_Result.push_back(Message_Log[i]);
			}
		}
	}
	else if (search_mode == 3) {
		string MessageBegin;
		cout << "\nEnter the beginning of message: ";
		cin.ignore(256, '\n');
		getline(cin, MessageBegin);
		for (size_t i = 0; i < Message_Log.size(); i++) {
			if (Compare(Message_Log[i], MessageBegin) == true) {
				Message_Result.push_back(Message_Log[i]);
			}
		}
	}


	return Message_Result;
	
	
}

vector <message> AutoSearch(vector<message>& Message_Log, const short int search_mode, const int program_mode) {
	vector <message> Message_Result;
	if (search_mode == 1) {
		Time StartTime, FinishTime;

		if (program_mode == Demonstration) {
			StartTime = { 00, 12, 56 };
			FinishTime = { 13, 01, 34 };
		}
		else {
			// generate random data
			StartTime.hour = rand() % 23;
			StartTime.min = rand() % 59;
			StartTime.second = rand() % 59;

			FinishTime.hour = rand() % 23;
			FinishTime.min = rand() % 59;
			FinishTime.second = rand() % 59;

			while (CompareTime(StartTime, FinishTime) == false) {
				StartTime.hour = rand() % 23;
				StartTime.min = rand() % 59;
				StartTime.second = rand() % 59;

				FinishTime.hour = rand() % 23;
				FinishTime.min = rand() % 59;
				FinishTime.second = rand() % 59;
			}
		}

		

		for (size_t i = 0; i < Message_Log.size(); i++) {
			if (Compare(Message_Log[i], StartTime, FinishTime) == true) {
				Message_Result.push_back(Message_Log[i]);
			}
		}
	}
	else if (search_mode == 2) {
		int MessageType;
		float LoadLevel;
		if (program_mode == Demonstration) {
			MessageType = info;
			LoadLevel = 0.5;
		}
		else {
			MessageType = rand() % 4;
			LoadLevel = (float)(rand() % 10000) / 10000;
		}
		

		for (size_t i = 0; i < Message_Log.size(); i++) {
			if (Compare(Message_Log[i], MessageType, LoadLevel) == true) {
				Message_Result.push_back(Message_Log[i]);
			}
		}
	}
	else if (search_mode == 3) {
		string MessageBegin;
		if (program_mode == Benchmark) {
			MessageBegin = Generate_Line();
		}
		else if (program_mode == Demonstration) {
			MessageBegin = "Mess";
		}
		
		for (size_t i = 0; i < Message_Log.size(); i++) {
			if (Compare(Message_Log[i], MessageBegin) == true) {
				Message_Result.push_back(Message_Log[i]);
			}
		}
	}
	return Message_Result;
}


Time Get_MessageTime() {
	Time UserTime;

	cout << "Enter a time hours:  ";
	cin >> UserTime.hour; Is_correct_value(UserTime.hour, 0, 23);

	cout << "Enter a time minutes:  ";
	cin >> UserTime.min; Is_correct_value(UserTime.min, 0, 59);

	cout << "Enter a time seconds:  ";
	cin >> UserTime.second; Is_correct_value(UserTime.second, 0, 59);

	return UserTime;
}

// time_1 > time_2 --> false
bool CompareTime(Time time_1, Time time_2){
	if (time_1.hour < time_2.hour) { return true; }
	else if (time_1.hour == time_2.hour) {
		if (time_1.min < time_2.min) { return true; }
		else if (time_1.min == time_2.min) {
			if (time_1.second < time_2.second) { return true; }
			else if (time_1.second == time_2.second) { return true; }
		}
	}
	return false;
}


int Get_MessageType() {
	int MessageType;
	cout << "Enter a Message Type (0 -- debug, 1 -- info, 2 -- warning, 3 -- error, 4 -- fatal):  ";
	cin >> MessageType; Is_correct_value(MessageType, 0, 4);

	return MessageType;
}

float Get_LoadLevel() {
	float LoadLevel;
	cout << "Enter a Load level (float [0,1]):  ";
	cin >> LoadLevel; Is_correct_value(LoadLevel, 0, 1);

	return LoadLevel;
}

bool Compare(message Message, Time StartTime, Time FinishTime) {

	if (CompareTime(StartTime, Message.time) && CompareTime(Message.time, FinishTime)) {
		return true;
	}

	return false;
}

bool Compare(message Message, int MessageType, float LoadLevel) {
	if (Message.type == MessageType && Message.load_level - LoadLevel >= 0) {
		return true;
	}
	return false;
}

bool Compare(message Message, string MessageStart) {
	if ((Message.text).length() < MessageStart.length()) { return false; }
	if (MessageStart == "") { 
		if (Message.text != MessageStart) { return false; }
	}
	else if ((Message.text).find(MessageStart) != 0) { return false; }
	return true;
}



void PrintSearchResult(vector<message>Search_Result) {
	if (Search_Result.size() == 0) {
		cout << "\nNothing was found\n";
		return;
	}
	cout << "\nResult of searching:\n\n";
	for (size_t i = 0; i < Search_Result.size(); i++) {
		print(Search_Result[i]);
	}
}


void Generate_newDB(vector<message>& Message_Log, const int SIZE) {
	message new_Message;
	for (size_t i = 0; i < SIZE; i++) {
		new_Message.id = i;
		new_Message.time.hour = rand() % 23;
		new_Message.time.min = rand() % 59;
		new_Message.time.second = rand() % 59;
		new_Message.date.day = rand() % 30 + 1;
		new_Message.date.month = rand() % 11 + 1;
		new_Message.date.year = rand() % 15 + 2006;
		new_Message.type = rand() % 4;
		new_Message.priority = rand() % 200;
		new_Message.load_level = (float)(rand() % 10000) / 10000;
		new_Message.text = Generate_Line();
		

		Message_Log.push_back(new_Message);
	}
	
}

string Generate_Line() {
	size_t size;
	size = rand() % 200;
	if (size == 0 || size == 1) { return ""; }

	string new_line;
	for (size_t i = 0; i < size - 1; i++) {
		new_line+= (char)(rand() % 79 + 47);
	}
	return new_line;
}


unsigned long int SizeOfFile(string filename) {
	ifstream File(filename, std::ifstream::ate | std::ifstream::binary);
	unsigned long int size_of_file = File.tellg();
	File.close();
	return size_of_file;
}

unsigned long int size_of_local_storage(vector<message>& Message_Log) {
	unsigned long int Size = 0;
	/*
	Size += Message_Log.capacity() * sizeof(message);
	for (size_t i = 0; i < Message_Log.size(); i++) {
		Size += (Message_Log[i].text).capacity();
	}
	*/
	for (size_t i = 0; i < Message_Log.size(); i++) {
		Size += sizeof(Message_Log[i]);
		Size += (Message_Log[i].text).capacity();
	}
	return Size;
}
