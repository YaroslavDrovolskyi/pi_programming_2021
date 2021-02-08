#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

struct date {
	int day, month, year;
};
struct Time {
	int hour, min, second;
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

short int ReadFromBIN(string FileName, vector<message>& Message_Log);
short int WriteInTEXT(vector<message>& Message_Log, string FileName, const size_t begin = 0, short int WriteMode = append_to_file);
short int ReadFromTEXT(string FileName, vector<message>& Message_Log);
message GetFromUser();
void ReadFromUser(vector<message>& Message_Log);
void PrintBD(vector<message>& Message_Log);
void ClearBD(vector<message>& Message_Log, string BINfile, string TEXTfile);

void print(message Message);


vector <message> Search(vector<message>& Message_Log);
Time Get_MessageTime();
bool CompareTime(Time time_1, Time time_2);
int Get_MessageType();
float Get_LoadLevel();
bool Compare(message Message, Time StartTime, Time FinishTime);
bool Compare(message Message, int MessageType, float LoadLevel);
bool Compare(message Message, string MessageStart);
void PrintSearchResult(vector<message>Search_Result);


void Generate_newDB(vector<message>& Message_Log, const int SIZE);
string Generate_Line();
vector <message> AutoSearch(vector<message>& Message_Log, const short int search_mode);





void WriteInBIN(message Message, string FileName);
void WriteInTEXT(message Message, string FileName);


int main() {
	
	cout << "Hello, dear user!\n Chose the mode, please:\n";
	string BINfile = "Messages log_BIN.txt";
	string TEXTfile = "Messages log_TEXT.txt";
	vector<message> Message_Log;
	int Mode = -1, next = -1;
	while (true) {
		cout << "===== MENU =====\n" << 
			"0 \tEXIT\n" <<
			"1 \tInteractive mode\n" <<
			"2 \tDemonstration mode\n" <<
			"3 \tBenchmark mode\n" << 
			"================\n";
		cin >> Mode;
		while (Mode < 0 || Mode > 3 || !cin.good()) {
			cout << "Enter valid value: \n";
			cin.clear();
			cin.ignore(256, '\n');
			cin >> Mode;
		}
		if (Mode == Turn_Back) { 
			
			return 0;
		}
		else if (Mode == Interactive) {
			while (true) {
				cout << "\n===== MENU =====\n" <<
					"Enter 0 to turn back\n" <<
					"1 \tfill the BD as new one\n" <<
					"2 \trestore BD from BIN file\n" <<
					"3 \trestore BD from TEXT file\n" <<
					"4 \tappend something to BD\n" <<
					"5 \tsearch\n" <<
					"6 \tprint BD\n" <<
					"7 \tclear BD\n" <<
					"================\n";
				cin >> next;
				while (next < 0 || next > 7 || !cin.good()) {
					cout << "Enter valid value: \n";
					cin.clear();
					cin.ignore(256, '\n');
					cin >> next;
				}

				if (next == 0) { break; }
				
				if (next == 1) {
					ClearBD(Message_Log, BINfile, TEXTfile);
					ReadFromUser(Message_Log);
					cout << "\nBD successfully created\n";
				}
				else if (next == 2) {
					Message_Log.clear();
					
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
					Message_Log.clear();

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
				else if (next == 6) {
					PrintBD(Message_Log);
				}
				else if (next == 7) {
					ClearBD(Message_Log, BINfile, TEXTfile);
					cout << "\nDB successfully cleared\n";
				}
			}
		}
		else if (Mode == Demonstration) {
			break;
		}
		else if (Mode == Benchmark) {
			if (Message_Log.empty()) { Message_Log.clear(); }
			cout << "Enter N = ";
			int N;
			cin >> N;
			while (N < 0 || !cin.good()) {
				cout << "Enter valid value: \n";
				cin.clear();
				cin.ignore(256, '\n');
				cin >> N;
			}

			unsigned int start_time, finish_time;
			float TEXT_time, BIN_time;
			vector<message> TEXT_Search_1, TEXT_Search_2, TEXT_Search_3,
				BIN_Search_1, BIN_Search_2, BIN_Search_3;


			start_time = clock();
			Generate_newDB(Message_Log, N);
			WriteInTEXT(Message_Log, "Benchmark_TEXT.txt", 0, file_as_new);
			Message_Log.clear();
			ReadFromTEXT("Benchmark_TEXT.txt", Message_Log);
			TEXT_Search_1 = AutoSearch(Message_Log, 1);
			TEXT_Search_2 = AutoSearch(Message_Log, 2);
			TEXT_Search_3 = AutoSearch(Message_Log, 3);
			finish_time = clock();
			TEXT_time = (float)(finish_time - start_time) / 1000;
			Message_Log.clear();
			
			start_time = clock();
			Generate_newDB(Message_Log, N);
			WriteInBIN(Message_Log, "Benchmark_BIN.txt", 0, file_as_new);
			Message_Log.clear();
			ReadFromBIN("Benchmark_BIN.txt", Message_Log);
			BIN_Search_1 = AutoSearch(Message_Log, 1);
			BIN_Search_2 = AutoSearch(Message_Log, 2);
			BIN_Search_3 = AutoSearch(Message_Log, 3);
			finish_time = clock();
			BIN_time = (float)(finish_time - start_time) / 1000;
			//WriteInTEXT(Message_Log, TEXTfile, 0, file_as_new);
			Message_Log.clear();

			cout << "\nFrom BIN Search_1 (time interval)\n";
			PrintSearchResult(BIN_Search_1);
			cout << "\nFrom BIN Search_2 (type and load level)\n";
			PrintSearchResult(BIN_Search_2);
			cout << "\nFrom BIN Search_3 (beginning of message)\n";
			PrintSearchResult(BIN_Search_3);
			
			cout << "\nFrom TEXT Search_1 (time interval)\n";
			PrintSearchResult(TEXT_Search_1);
			cout << "\nFrom TEXT Search_2 (type and load level)\n";
			PrintSearchResult(TEXT_Search_2);
			cout << "\nFrom TEXT Search_3 (beginning of message)\n";
			PrintSearchResult(TEXT_Search_3);
			
			


			
			cout << "\n\n\n\n===============RESULTS===============\n";
			
			cout << "Time with BIN files: " << BIN_time << endl;
			cout << "Time with TEXT files: " << TEXT_time << endl;

			break;
		}
		else { cout << "Something is wrong"; return -1; }



	}



	message mes1{ 1, "First message", {10, 10, 2000},{12, 00, 00}, info, 200, 0.100 };
	message mes2{2, "Second Message djg", {10, 10, 2000},{12, 00, 00}, debug, 0, 0.11 };
	message mes3{3, "Third Message #3", {10, 10, 2020},{12, 00, 00}, info, 200, 0.100 };

	system("pause");
	return 0;
}

short int WriteInBIN (vector<message>& Message_Log, string FileName, const size_t begin, short int WriteMode) {
	ofstream OutputFile;
	if (WriteMode == append_to_file) { OutputFile.open(FileName, ofstream::app); }
	else if (WriteMode == file_as_new) { OutputFile.open(FileName); }

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
	InputFile.open(FileName);
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

message GetFromUser() {
	message Message;
	cin.ignore(256,'\n');
	cout << "Enter a Message:  ";
	getline(cin, Message.text);
	

	//cout << "Enter a date in format DD MM YY:  ";
	cout << "Enter a day:  ";
	cin >> Message.date.day;
	while (Message.date.day <= 0 || Message.date.day > 31 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256,'\n');
		
		cin >> Message.date.day;
	}

	cout << "Enter a month:  ";
	cin >> Message.date.month;
	while (Message.date.month <= 0 || Message.date.month > 12 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> Message.date.month;
	}

	cout << "Enter a year:  ";
	cin >> Message.date.year;
	while (Message.date.year <= 0 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> Message.date.year;
	}

	cout << "Enter a time hours:  ";
	cin >> Message.time.hour;
	while (Message.time.hour < 0 || Message.time.hour > 23 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> Message.time.hour;
	}

	cout << "Enter a time minutes:  ";
	cin >> Message.time.min;
	while (Message.time.min < 0 || Message.time.min > 59 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> Message.time.min;
	}

	cout << "Enter a time seconds:  ";
	cin >> Message.time.second;
	while (Message.time.second < 0 || Message.time.min > 59 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> Message.time.second;
	}


	cout << "Enter a Message Type (0 -- debug, 1 -- info, 2 -- warning, 3 -- error, 4 -- fatal):  ";
	cin >> Message.type;
	while (Message.type < 0 || Message.type > 4 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> Message.type;
	}


	cout << "Enter a Message priotity (int [0,200]):  ";
	cin >> Message.priority;
	while (Message.priority < 0 || Message.priority > 200 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> Message.priority;
	}

	cout << "Enter a Load level (float [0,1]):  ";
	cin >> Message.load_level;
	while (Message.load_level - 0 < 0 || Message.load_level -1 > 0 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> Message.load_level;
	}


	return Message;
}

void ReadFromUser(vector<message>& Message_Log) {
	// doesn't matter, if ReadFromTEXT will return 0 or -1 
	if (Message_Log.size() == 0) { ReadFromTEXT("Messages log_TEXT.txt", Message_Log); }
	message Message;
	size_t begin = Message_Log.size();
	bool next = true;
	while (next == true) {
		Message = GetFromUser();
		if (Message_Log.size() == 0) { Message.id = 0; }
		else {Message.id = Message_Log[Message_Log.size() - 1].id + 1;}
		Message_Log.push_back(Message);
		
		cout << "0 -- finish, 1 -- continue\n";
		cin >> next;
		while (next < 0 || next > 1 || !cin.good()) {
			cout << "Enter valid value: \n";
			cin.clear();
			cin.ignore(256, '\n');
			cin >> next;
		}
	}

	// Write added info in files
	WriteInTEXT(Message_Log, "Messages log_TEXT.txt", begin);
	WriteInBIN(Message_Log, "Messages log_BIN.txt", begin);
}

void print(message Message) {
	cout << "ID: " << Message.id << endl;
	cout << "Text: " << Message.text << endl;
	cout << "Time: " << Message.time.hour << ' ' << Message.time.min << ' ' << Message.time.second << endl;
	cout << "Date: " << Message.date.day << ' ' << Message.date.month << ' ' << Message.date.year << endl;
	cout << "Type: " << Message.type << endl;
	cout << "Priority: " << Message.priority << endl;
	cout << "Load level: " << Message.load_level << endl;
	cout << "\n============================\n";
}

void PrintBD(vector<message>& Message_Log) {
	if (Message_Log.size() == 0) {
		if (ReadFromTEXT("Messages log_TEXT.txt", Message_Log) != 1) {
			if (ReadFromBIN("Messages log_BIN.txt", Message_Log) != 1) {
				cout << "Nothing in local, nothing in TEXT and BIN files or something with them\n";
				return;
			}
			else {
				cout << "Nothing in local, TEXT, printed from BINfile\n";
			}
		}
		else {
			cout << "Nothing in local, printed from TEXTfile\n";
		}
	}
	for (size_t i = 0; i < Message_Log.size(); i++) {
		print(Message_Log[i]);
	}
}
void ClearBD(vector<message>& Message_Log, string BINfile, string TEXTfile) {
	ofstream FileBIN, FileTEXT;
	Message_Log.clear();
	FileBIN.open(BINfile, ofstream::out | ofstream::trunc);
	FileTEXT.open(TEXTfile, ofstream::out | ofstream::trunc);
	FileBIN.close();
	FileTEXT.close();
}

vector <message> Search(vector<message>& Message_Log) {
	vector <message> Message_Result;
	short int search_mode;
	cout << "\nChoose criteriors to search:\n" <<
		"1 - Time interval\n" <<
		"2 - Type and load level\n" <<
		"3 - Beginning of message\n";

	cin >> search_mode;
	while (search_mode < 1 || search_mode > 3 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> search_mode;
	}
	if (search_mode == 1) {
		Time StartTime, FinishTime;
		StartTime = Get_MessageTime();
		FinishTime = Get_MessageTime();
		while (CompareTime(StartTime, FinishTime) == false) {
			StartTime = Get_MessageTime();
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

vector <message> AutoSearch(vector<message>& Message_Log, const short int search_mode) {
	vector <message> Message_Result;
	if (search_mode == 1) {
		Time StartTime, FinishTime;

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

		for (size_t i = 0; i < Message_Log.size(); i++) {
			if (Compare(Message_Log[i], StartTime, FinishTime) == true) {
				Message_Result.push_back(Message_Log[i]);
			}
		}
	}
	else if (search_mode == 2) {
		int MessageType;
		float LoadLevel;
		MessageType = rand() % 4;
		LoadLevel = (float)(rand() % 10000) / 10000;

		for (size_t i = 0; i < Message_Log.size(); i++) {
			if (Compare(Message_Log[i], MessageType, LoadLevel) == true) {
				Message_Result.push_back(Message_Log[i]);
			}
		}
	}
	else if (search_mode == 3) {
		string MessageBegin = Generate_Line();
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
	cin >> UserTime.hour;
	while (UserTime.hour < 0 || UserTime.hour > 23 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> UserTime.hour;
	}

	cout << "Enter a time minutes:  ";
	cin >> UserTime.min;
	while (UserTime.min < 0 || UserTime.min > 59 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> UserTime.min;
	}

	cout << "Enter a time seconds:  ";
	cin >> UserTime.second;
	while (UserTime.second < 0 || UserTime.second > 59 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> UserTime.second;
	}
	return UserTime;
}

// time_1 > time_2 --> false
bool CompareTime(Time time_1, Time time_2){
	if (time_1.hour <= time_2.hour) {
		if (time_1.min <= time_2.min) {
			if (time_1.second <= time_2.second) {
				return true;
			}
		}
	}
	return false;
}


int Get_MessageType() {
	int MessageType;
	cout << "Enter a Message Type (0 -- debug, 1 -- info, 2 -- warning, 3 -- error, 4 -- fatal):  ";
	cin >> MessageType;
	while (MessageType < 0 || MessageType > 4 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> MessageType;
	}
	return MessageType;
}

float Get_LoadLevel() {
	float LoadLevel;
	cout << "Enter a Load level (float [0,1]):  ";
	cin >> LoadLevel;
	while (LoadLevel - 0 < 0 || LoadLevel - 1 > 0 || !cin.good()) {
		cout << "Enter valid value: \n";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> LoadLevel;
	}
	return LoadLevel;

}








bool Compare(message Message, Time StartTime, Time FinishTime) {
	if (Message.time.hour >= StartTime.hour && Message.time.hour <= FinishTime.hour) {
		if (Message.time.min >= StartTime.min && Message.time.min <= FinishTime.min) {
			if (Message.time.second >= StartTime.second && Message.time.second <= FinishTime.second) {
				return true;
			}
			else { return false; }
		}
		else { return false; }
	}
	else { return false; }
}

bool Compare(message Message, int MessageType, float LoadLevel) {
	if (Message.type == MessageType && Message.load_level >= LoadLevel) {
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
	int size;
	size = rand() % 200;
	if (size == 0 || size == 1) { return ""; }

	string new_line;
	for (size_t i = 0; i < size - 1; i++) {
		new_line+= (char)(rand() % 79 + 47);
	}
	return new_line;
}





void WriteInTEXT(message Message, string FileName) {
	ofstream File;
	File.open(FileName, ofstream::app);
	if (!File.is_open()) {
		cout << "Error!";
		return;
	}
	File << '\n' << Message.id << '\t';
	File << Message.date.day << ' ';
	File << Message.date.month << ' ';
	File << Message.date.year << '\t';
	File << Message.time.hour << ' ';
	File << Message.time.min << ' ';
	File << Message.time.second << '\t';
	File << Message.type << '\t';
	File << Message.priority << '\t';
	File << Message.load_level << '\n';
	File << Message.text;

	File.close();
}
void WriteInBIN(message Message, string FileName) {
	ofstream OutputFile;
	OutputFile.open(FileName, ofstream::app);
	if (!OutputFile.is_open()) {
		cout << "Error!";
		return;
	}

	OutputFile.write((char*)&(Message.id), sizeof(int));

	size_t size = (Message.text).size();
	OutputFile.write((char*)&size, sizeof(size));
	OutputFile.write(&(Message.text)[0], size);  //text

	OutputFile.write((char*)&(Message.date), sizeof(date));
	OutputFile.write((char*)&(Message.time), sizeof(Time));
	OutputFile.write((char*)&(Message.type), sizeof(MessageType));
	OutputFile.write((char*)&(Message.priority), sizeof(short int));
	OutputFile.write((char*)&(Message.load_level), sizeof(float));

	OutputFile.close();
}