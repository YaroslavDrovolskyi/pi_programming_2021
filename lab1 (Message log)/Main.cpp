#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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


struct message {
	int id;
	string text;
	date date;
	Time time;
	int type; // debug, info, warning, error, fatal
	short int priority; // [0;200]
	float load_level; // [0;1]

};

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

void WriteInBIN(vector<message>& Message_Log, string FileName);
void WriteInBIN(message Message, string FileName);
void ReadFromBIN(string FileName, vector<message>& Message_Log);
void WriteInTEXT(vector<message>& Message_Log, string FileName);
void WriteInTEXT(message Message, string FileName);
void ReadFromTEXT(string FileName, vector<message>& Message_Log);
message ReadFromUser();
void ReadFomUser(vector<message>& Message_Log);
void PrintBD(vector<message>& Message_Log);
void ClearBD(vector<message>& Message_Log, string BINfile, string TEXTfile);


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
		if (Mode == Turn_Back) { return 0; }
		else if (Mode == Interactive) {
			while (true) {
				cout << "===== MENU =====\n" <<
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
				if (next == 0) { break; }
				
				if (next == 1) {
					ReadFomUser(Message_Log);
					cout << "\nBD successfully created\n";
				}
				else if (next == 2) {
					ReadFromBIN(BINfile, Message_Log);
					cout << "\nBD successfully restored from BIN file\n";
				}
				else if (next == 3) {
					ReadFromTEXT(TEXTfile, Message_Log);
					cout << "\nBD successfully restored from TEXT file\n";
				}
				else if (next == 4) {
					ReadFomUser(Message_Log);
					cout << "\nBD successfully added\n";
				}
				else if (next == 5) {

				}
				else if (next == 6) {
					PrintBD(Message_Log);
				}
				else if (next == 7) {
					ClearBD(Message_Log, BINfile, TEXTfile);
				}
			}
		}
		else if (Mode == Demonstration) {
			break;
		}
		else if (Mode == Benchmark) {
			cout << "Enter N = \n";
			break;
		}
		else { cout << "Something is wrong"; return -1; }



	}



	message mes1{1, "First message", {10, 10, 2000},{12, 00, 00}, info, 200, 0.100 };
	message mes2{2, "Second Message djg", {10, 10, 2000},{12, 00, 00}, debug, 0, 0.11 };
	message mes3{3, "Third Message #3", {10, 10, 2020},{12, 00, 00}, info, 200, 0.100 };

	vector<message> MessageStart_Log{mes1, mes2, mes3};

	//WriteInBIN(MessageStart_Log, "Messages log_BIN.txt");
	
	//vector<message> Message_Log;
	//ReadFromBIN("Messages log_BIN.txt", Message_Log);

	for (size_t i = 0; i < Message_Log.size(); i++) {
	//	print(Message_Log[i]);
	}
	
	//WriteInTEXT(Message_Log, "Messages log_TEXT.txt");

	vector<message> Messages_FromText;

	ReadFromTEXT("Messages log_TEXT.txt", Messages_FromText);

	for (size_t i = 0; i < Messages_FromText.size(); i++) {
		print(Messages_FromText[i]);
	}

	
	
	cout << "\nOK!";
	
	return 0;
}

void WriteInBIN (vector<message>& Message_Log, string FileName) {
	ofstream OutputFile;
	OutputFile.open(FileName, ofstream::app);
	if (!OutputFile.is_open()) {
		cout << "Error!";
		return ;
	}
	for (size_t i = 0; i < Message_Log.size(); i++) {
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


void ReadFromBIN(string FileName, vector<message>& Message_Log) {
	ifstream InputFile;
	message Message;
	InputFile.open(FileName);
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
}

void WriteInTEXT(vector<message>& Message_Log, string FileName) {
	ofstream File;
	File.open(FileName, ofstream::app);
	if (!File.is_open()) {
		cout << "Error!";
		return;
	}
	for (size_t i = 0; i < Message_Log.size(); i++) {
		File << Message_Log[i].text << '\n';
		File << Message_Log[i].id << '\t';
		File << (Message_Log[i].date).day << ' ';
		File << (Message_Log[i].date).month << ' ';
		File << (Message_Log[i].date).year << '\t';
		File << Message_Log[i].time.hour << ' ';
		File << Message_Log[i].time.min << ' ';
		File << Message_Log[i].time.second << '\t';
		File << Message_Log[i].type << '\t';
		File << Message_Log[i].priority << '\t';
		File << Message_Log[i].load_level << '\n';
	}
	File.close();
}

void WriteInTEXT(message Message, string FileName) {
	ofstream File;
	File.open(FileName, ofstream::app);
	if (!File.is_open()) {
		cout << "Error!";
		return;
	}
	File << Message.text << '\n';
	File << Message.id << '\t';
	File << Message.date.day << ' ';
	File << Message.date.month << ' ';
	File << Message.date.year << '\t';
	File << Message.time.hour << ' ';
	File << Message.time.min << ' ';
	File << Message.time.second << '\t';
	File << Message.type << '\t';
	File << Message.priority << '\t';
	File << Message.load_level << '\n';

	File.close();
}

void ReadFromTEXT(string FileName, vector<message>& Message_Log) {
	ifstream File;
	File.open(FileName);
	message Message;
	while (!File.eof()) {
		getline(File, Message.text);
		if (File.eof() || Message.text == "") { break; }
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
		File.ignore();
	
		Message_Log.push_back(Message);
	}
	
	File.close();
	
}

message ReadFromUser() {
	message Message;
	cin.ignore(256,'\n');
	cout << "Enter a Message:  ";
	getline(cin, Message.text);
	cout << "Enter a Message ID:  ";
	cin >> Message.id;
	cout << "Enter a date in format day month year:  ";
	cin >> Message.date.day;
	cin >> Message.date.month;
	cin >> Message.date.year;
	cout << "Enter a time in format hours minutes seconds:  ";
	cin >> Message.time.hour;
	cin >> Message.time.min;
	cin >> Message.time.second;
	cout << "Enter a Message Type (0 -- debug, 1 -- info, 2 -- warning, 3 -- error, 4 -- fatal):  ";
	cin >> Message.type;
	cout << "Enter a Message priotity:  ";
	cin >> Message.priority;
	cout << "Enter a Load level:  ";
	cin >> Message.load_level;
	return Message;
}

void ReadFomUser(vector<message>& Message_Log) {
	message Message;
	bool next = true;
	//getline(cin, Message.text);
	while (next == true) {
		Message = ReadFromUser();
		Message_Log.push_back(Message);
		WriteInTEXT(Message, "Messages log_TEXT.txt");
		WriteInBIN(Message, "Messages log_BIN.txt");
		cout << "0 -- finish, 1 -- continue\n";
		cin >> next;
	}
}

void PrintBD(vector<message>& Message_Log) {
	if (Message_Log.size() <= 0) { 
		cout << "Nothing in BD\n";
		ReadFromTEXT("Messages log_TEXT.txt", Message_Log);
		if (Message_Log.size() <= 0) { cout << "Nothing in BD\n"; return; }
		else { cout << "Printed from TEXTfile\n"; }
	}
	
	for (size_t i = 0; i < Message_Log.size(); i++) {
		print(Message_Log[i]);
	}
}
void ClearBD(vector<message>& Message_Log, string BINfile, string TEXTfile) {
	ofstream FileBIN, FileTEXT;
	Message_Log.clear();
	//FileBIN.open(BINfile, ofstream::out | ofstream::trunc);
	//FileTEXT.open(TEXTfile, ofstream::out | ofstream::trunc);
	//FileBIN.close();
	//FileTEXT.close();
}