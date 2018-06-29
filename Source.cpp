#include <Windows.h>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>

using namespace std;



#define Up 'w'
#define Use 'u'
#define Back 'b'
#define Down 's'
#define Chose ' '

#define Pointer " ---->"
#define WindowSize 20
#define MaximalDescriptionLength 25

#define Symbols "0123456789abcdef"



std::string Repeat(std::string val, short times);
std::string Repeat(char val, short times);
std::string Cut(std::string text, short length);
std::string Hex(std::string inp);
std::string Byte(std::string inp);

//I was feeling laze to make this program perfect as i usually do so i didn`t finish it.



template <typename datatype>
struct LPart {
	LPart<datatype>* pointer;
	datatype payload;
};
template <typename datatype>
class List {
private:
	LPart<datatype>* start;
	bool empty = 1;

public:
	int length = 0;

	List();
	void append(datatype dlt);
	void add(datatype dlt); // it`s like append, but adds new element to the start of the list (instead of end)
	void del(short ind);
	datatype pop();
	datatype get(short ind);
	datatype* getlink(short ind);
	std::string str(std::string sep);
};
template <typename datatype>
List<datatype>::List() {
}
template <typename datatype>
void List<datatype>::append(datatype dlt) {
	if (empty) {
		start = new LPart<datatype>;
		(*start).payload = dlt;
		(*start).pointer = start;

		empty = 0;
	}
	else {
		if (length == 0) {
			(*start).payload = dlt;
			(*start).pointer = start;
		}
		else {
			LPart<datatype>* appendix = new LPart<datatype>;
			(*appendix).payload = dlt;
			(*appendix).pointer = appendix;

			LPart<datatype>* last = start;
			while ((*last).pointer != last) {
				last = (*last).pointer;
			}
			(*last).pointer = appendix;
		}
	}

	length += 1;
}
template <typename datatype>
void List<datatype>::add(datatype dlt) {
	if (empty) {
		start = new LPart<datatype>;
		(*start).payload = dlt;
		(*start).pointer = start;

		empty = 0;
	}
	else {
		if (length == 0) {
			(*start).payload = dlt;
			(*start).pointer = start;
		}
		else {
			LPart<datatype>* appendix = new LPart<datatype>;
			(*appendix).payload = dlt;
			(*appendix).pointer = start;

			start = appendix;
		}
	}

	length += 1;
}
template <typename datatype>
void List<datatype>::del(short ind) {
	length -= 1;

	if (ind == 0) {
		start = start->pointer;
	}
	else {
		LPart<datatype>* part = start;
		LPart<datatype> newpointer;

		for (short q = 0; q < ind - 1; q++) {
			part = (*part).pointer;
		}

		(*part).pointer = (*(*part).pointer).pointer;
	}
}
template <typename datatype>
datatype List<datatype>::pop() {
	length -= 1;

	datatype out = (*start).payload;
	start = (*start).pointer;
	return out;
}
template <typename datatype>
datatype List<datatype>::get(short ind) {
	LPart<datatype>* last = start;
	for (short q = 0; q < ind; q++) {
		last = (*last).pointer;
	}

	return (*last).payload;
}
template <typename datatype>
datatype* List<datatype>::getlink(short ind) {
	LPart<datatype>* last = start;
	for (short q = 0; q < ind; q++) {
		last = (*last).pointer;
	}

	return &(*last).payload;
}
template <typename datatype>
std::string List<datatype>::str(std::string sep) {
	string out = "";

	if (length > 0) {
		for (short q = 0; q < length; q++) {
			out += get(q);
			out += sep;
		}
		out.erase(out.end() - 1);
	}

	return out;
}
List<std::string> Split(std::string text, char sep);

class Filesystem {
private:
	void strpart(std::string* out, Filesystem fsys, short level);
	void codepart(std::string* out, Filesystem fsys, std::string path);
public:
	std::string name;
	List<Filesystem> containment;
	std::string payload;
	bool directory;

	Filesystem();
	void addfile(std::string val, List<std::string> path);
	void delfile(List <std::string> path);
	void adddir(List<std::string> path, std::string description);
	void deldir(List<std::string> path);
	void load(std::string inp);
	void addsys(bool directory, List<std::string> path, std::string payload);
	void rename(List<std::string> path, std::string newname);
	std::string str();
	std::string code();
	Filesystem get(List<std::string> path);
};
Filesystem::Filesystem() {
	name = "Main";
	payload = "The main directory";
	directory = 1;
}
void Filesystem::addfile(std::string val, List<std::string> path) {
	if (path.length == 1) {
		bool found = 0;
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == path.get(0)) {
				found = 1;
				break;
			}
		}

		if(found){
			for (short q = 0; q < containment.length; q++) {
				if (containment.get(q).name == path.get(path.length - 1)) {
					(*containment.getlink(q)).payload = payload;
				}
			}
		}
		else {
			Filesystem newfile;
			newfile.name = path.get(0);
			newfile.payload = val;
			newfile.directory = 0;

			containment.append(newfile);
		}
	}
	else {
		string dirname = path.pop();

		bool found = 0;
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == dirname) {
				(*containment.getlink(q)).addfile(val, path);
				found = 1;
				break;
			}
		}
		if (!found) {
			cout << "Invalid filesystem path" << endl;
			_getch();
			throw std::invalid_argument("Invalid filesystem path");
		}
	}
}
void Filesystem::delfile(List<std::string> path) {
	if (path.length == 1) {
		bool found = 0;
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == path.get(0)) {
				containment.del(q);
				found = 1;
			}
		}
		if (!found) {
			cout << "Invalid filesystem path" << endl;
			_getch();
			throw std::invalid_argument("Invalid filesystem path");
		}
	}
	else {
		string dirname = path.pop();

		bool found = 0;
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == dirname) {
				(*containment.getlink(q)).delfile(path);
				found = 1;
				break;
			}
		}
		if (!found) {
			cout << "Invalid filesystem path" << endl;
			_getch();
			throw std::invalid_argument("Invalid filesystem path");
		}
	}
}
void Filesystem::adddir(List<std::string> path, std::string description) {
	if (path.length == 1) {
		bool exists = 0;
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == path.get(path.length - 1)) {
				exists = 1;
				break;
			}
		}

		if (!exists) {
			Filesystem newdir;
			newdir.name = path.get(0);
			newdir.payload = description;
			newdir.directory = 1;

			containment.append(newdir);
		}
		else {
			for (short q = 0; q < containment.length; q++) {
				if (containment.get(q).name == path.get(path.length - 1)) {
					(*containment.getlink(q)).payload = description;
				}
			}
		}
	}
	else {
		string dirname = path.pop();
		
		bool found = 0;
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == dirname) {
				(*containment.getlink(q)).adddir(path, description);
				found = 1;
				break;
			}
		}
		if (!found) {
			cout << "Invalid filesystem path" << endl;
			_getch();
			throw std::invalid_argument("Invalid filesystem path");
		}
	}
}
void Filesystem::deldir(List<std::string> path) {
	if (path.length == 1) {
		bool found = 0;
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == path.get(0)) {
				containment.del(q);
				found = 1;
			}
		}
		if (!found) {
			cout << "Invalid filesystem path" << endl;
			_getch();
			throw std::invalid_argument("Invalid filesystem path");
		}
	}
	else {
		string dirname = path.pop();

		bool found = 0;
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == dirname) {
				(*containment.getlink(q)).deldir(path);
				found = 1;
				break;
			}
		}
		if (!found) {
			cout << "Invalid filesystem path" << endl;
			_getch();
			throw std::invalid_argument("Invalid filesystem path");
		}
	}
}
void Filesystem::strpart(std::string* out, Filesystem fsys, short level) {
	*out += Repeat("    ", level);

	if (fsys.directory) {
		(*out) += fsys.name;
		(*out) += ":{\n";

		for (short q = 0; q < fsys.containment.length; q++) {
			strpart(out, fsys.containment.get(q), level + 1);
		}

		(*out) += Repeat("    ", level);
		(*out) += "} //";
		(*out) += Cut(fsys.payload, MaximalDescriptionLength);
	}
	else {
		(*out) += fsys.name;
	}

	*out += '\n';
}
std::string Filesystem::str() {
	string out = "";

	if (directory) {
		out += name;
		out += "{\n";
		for (short q = 0; q < containment.length; q++) {
			strpart(&out, containment.get(q), 1);
		}
		out += "} //";
		out += Cut(payload, MaximalDescriptionLength);
	}
	else {
		out += name;
		out += '\n';
		out += Repeat("-", name.length());
		out += '\n';
		out += payload;
	}

	return out;
}
void Filesystem::codepart(std::string* out, Filesystem fsys, std::string path) {
	for (short q = 0; q < fsys.containment.length; q++) {
		if (path.length() == 0) {
			codepart(out, fsys.containment.get(q), fsys.name);
		}
		else {
			codepart(out, fsys.containment.get(q), path + '/' + fsys.name);
		}
	}

	*out += to_string(fsys.directory) + '|';
	if (path.length() == 0) {
		*out += (Hex(fsys.name) + "|");
	}
	else {
		*out += Hex(path + '/' + fsys.name) + '|';
	}
	*out += Hex(fsys.payload);
	*out += '\n';
}
std::string Filesystem::code() {
	string out = "";

	for (short q = 0; q < containment.length; q++) {
		codepart(&out, containment.get(q), "");
	}

	return out;
}
void Filesystem::load(std::string inp) {
	List<string> lst = Split(inp, '\n');

	List<string> fsys;
	for (short q = 0; q < lst.length; q++) {
		fsys = Split(lst.get(q), '|');

		if (fsys.length == 3) {
			bool dir = (fsys.get(0) == "1");
			List<string> path = Split(Byte(fsys.get(1)), '/');
			string payload = Byte(fsys.get(2));
			addsys(dir, path, payload);
		}
	}
}
void Filesystem::addsys(bool directory, List<std::string> path, std::string payload) {
	if (path.length == 0) {
		return;
	}

	if (path.length > 1) {
		List<string> curpath;
		short n = 0;
		bool exists;
		while (curpath.length < path.length - 1) {
			curpath.append(path.get(n));
			adddir(curpath, "");
			n += 1;
		}
	}

	if (directory) {
		adddir(path, payload);
	}
	else {
		addfile(payload, path);
	}
}
Filesystem Filesystem::get(List<std::string> path) {
	if (path.length == 0) {
		Filesystem out;
		out.containment = containment;
		out.directory = directory;
		out.name = name;
		out.payload = payload;
		return out;
	}

	if (path.length == 1) {
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == path.get(0)) {
				return containment.get(q);
			}
		}

		cout << path.get(0) << endl;
		cout << "Invalid filesystem path" << endl;
		_getch();
		throw std::invalid_argument("Invalid filesystem path");
	}
	else {
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == path.get(0)) {
				path.pop();
				return (containment.get(q)).get(path);
			}
		}

		cout << "Invalid filesystem path" << endl;
		cout << "Path:" << path.str("/") << endl;
		_getch();
		throw std::invalid_argument("Invalid filesystem path");
	}
}
void Filesystem::rename(List<std::string> path, std::string newname) {
	if (path.length == 0) {
		name = newname;
	}

	if (path.length == 1) {
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == path.get(0)) {
				(*containment.getlink(q)).name = newname;
				return;
			}
		}

		cout << path.get(0) << endl;
		cout << "Invalid filesystem path" << endl;
		_getch();
		throw std::invalid_argument("Invalid filesystem path");
	}
	else {
		for (short q = 0; q < containment.length; q++) {
			if (containment.get(q).name == path.get(0)) {
				path.pop();
				(*containment.getlink(q)).rename(path, newname);
				return;
			}
		}

		cout << "Invalid filesystem path" << endl;
		cout << "Path:" << path.str("/") << endl;
		_getch();
		throw std::invalid_argument("Invalid filesystem path");
	}
}



List<std::string> Split(std::string text, char sep) {
	List<string> out;
	text += sep;

	string buffer;
	for (short q = 0; q < text.length(); q++) {
		if (text[q] == sep) {
			out.append(buffer);
			buffer = "";
		}
		else {
			buffer += text[q];
		}
	}

	return out;
}

std::string Repeat(std::string val, short times) {
	string out = "";

	for (short q = 0; q < times; q++) {
		out += val;
	}

	return out;
}
std::string Repeat(char val, short times) {
	string out = "";

	for (short q = 0; q < times; q++) {
		out += val;
	}

	return out;
}
std::string Cut(std::string text, short length) {
	string out = "";

	for (short q = 0; q < min(length, text.length()); q++) {
		out += text[q];
	}
	if (text.length() > length) {
		out += "...";
	}

	return out;
}

int Find(const char* text, char c) {
	int q = 0;
	while (1) {
		if (text[q] == c) {
			return q;
		}
		q++;
	}
}

std::string Hex(std::string inp) {
	string out = "";

	short n;
	char a, b;
	for (int q = 0; q < inp.length(); q++) {
		n = int(inp[q]);
		a = n / 16;
		b = n % 16;
		out += Symbols[a];
		out += Symbols[b];
	}

	return out;
}
std::string Byte(std::string inp) {
	string out = "";

	char a, b;
	for (int q = 0; q < inp.length() / 2; q++) {
		a = Find(Symbols, inp[q * 2]);
		b = Find(Symbols, inp[q * 2 + 1]);
		out += char(a * 16 + b);
	}

	return out;
}

List<std::string> Getfiles(string folder)
{
	// I don`t know how it works. Just found it in internet and modified.
	List<std::string> out;

	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if ((string(fd.cFileName) != ".") * (string(fd.cFileName) != "..")) {
				out.append(string(fd.cFileName));
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	return out;
}
std::string Readfile(std::string path) {
	string out = "";

	ifstream file;
	file.open(path);
	string buffer;
	while (!file.eof()) {
		getline(file, buffer);
		out += buffer;
		out += '\n';
	}
	out.erase((out.end() - 1));

	return out;
}
void Writefile(std::string path, std::string text) {
	ofstream file;
	file.open(path);
	for (int q = 0; q < text.length(); q++) {
		file << text[q];
	}
	file.close();
}

std::string Findfile() {
	string out = "C:";

	char key = 0;
	short adr = 0;
	short start, end;
	start = 0;
	end = WindowSize;
	while (1){
		List<string> files = Getfiles(out);
		if (adr < start) {
			if (key == Up) {
				start = adr;
				end = start + WindowSize;
			}
		}
		if (adr >= start + WindowSize) {
			if (key == Down) {
				end = adr + 1;
				start = end - WindowSize;
			}
		}

		system("cls");
		cout << "Current directory: " << out << endl;
		cout << "Chose file or directory" << endl;
		cout << "-----------------------" << endl;
		if (files.length > 0) {
			for (short q = start; q < end; q++) {
				if (q < files.length) {
					if (q == adr) {
						cout << Pointer;
					}
					else {
						cout << Repeat(' ', strlen(Pointer));
					}
					cout << files.get(q) << endl;
				}
				else {
					cout << endl;
				}
			}
		}
		else {
			cout << "(Empty)" << endl;
			for (short q = start; q < end - 1; q++) {
				cout << endl;
			}
		}
		cout << "-----------------------" << endl;
		cout << "To move press '" << Up << "' and '" << Down << "' buttons." << endl;
		cout << "To chose press '" << Chose << "' button." << endl;
		cout << "To go back press '" << Back << "' button." << endl;

		key = _getch();
		switch (key) {
		case Up:
		{
			if (adr > 0) {
				adr -= 1;
			}
			break;
		}
		case Down:
		{
			if (adr < files.length - 1) {
				adr += 1;
			}
			break;
		}
		case Chose:
		{
			start = 0;
			end = WindowSize;

			bool file = 0;
			for (short q = 0; q < files.get(adr).length(); q++) {
				if (files.get(adr)[q] == '.') {
					file = 1;
					break;
				}
			}

			if (file) {
				system("cls");
				cout << "Chose file '" << out << '/' << files.get(adr) << "'?" << endl;
				cout << "Press '" << Chose << "' to continue" << endl;
				cout << "Press '" << Back << "' to cancel" << endl;

				bool b = 1;
				while (b) {
					switch (_getch()) {
					case Chose:
						out += '/';
						out += files.get(adr);
						return out;
					case Back:
						b = 0;
						break;
					}
				}
			}
			else {
				out += '/';
				out += files.get(adr);
				adr = 0;
			}
			break;
		}
		case Back:
		{
			start = 0;
			end = WindowSize;
			adr = 0;

			List<string> lst = Split(out, '/');
			if (lst.length > 1) {
				lst.del(lst.length - 1);
			}
			out = lst.str("/");
			break;
		}
		}
	}

	return out;
}

void Change(Filesystem* files, List<std::string> path, std::string sourcefile) {
	cout << "Options:" << endl;
	if ((*files).get(path).directory) {
		cout << "\trename" << endl;
		cout << "\tdelete" << endl;
		cout << "\tcreate new file" << endl;
		cout << "\twrite new data" << endl;
	}
	else {
		cout << "\trename" << endl;
		cout << "\tdelete" << endl;
		cout << "\twrite new data" << endl;
		cout << "\textract" << endl;
	}
	cout << endl;
	cout << "Enter chosen option name below" << endl;

	string option;
	getline(cin, option);

	if (option == "create new file") {
		if ((*files).get(path).directory) {
			system("cls");

			string payload;
			payload = Readfile(Findfile());

			string name;
			bool brk;
			while (1) {
				cout << "Enter new file name" << endl;
				getline(cin, name);

				brk = 1;
				for (short q = 0; q < name.length(); q++) {
					if ((name[q] == '/') or (name[q] == '|')) {
						cout << "Name must not contain '/' and '|' symbols." << endl;
						_getch();
						brk = 0;
					}
				}
				if (brk) {
					break;
				}
			}

			path.append(name);
			(*files).addfile(payload, path);
			path.del(path.length - 1);
		}
		else {
			system("cls");
			cout << "Must chose directory to create a file" << endl;
			_getch();
		}
	}
	if (option == "rename") {
		system("cls");

		string name;
		bool brk;
		while (1) {
			cout << "Enter new dir/file name" << endl;
			getline(cin, name);

			brk = 1;
			for (short q = 0; q < name.length(); q++) {
				if ((name[q] == '/') or (name[q] == '|')) {
					cout << "Name must not contain '/' and '|' symbols." << endl;
					_getch();
					brk = 0;
				}
			}
			if (brk) {
				break;
			}
		}

		(*files).rename(path, name);
	}
	if (option == "delete") {
		string answer;
		cout << "Do you realy want do delete " << path.str("/") << " file/directory?" << endl;
		cout << "Enter YES to delete." << endl;
		getline(cin, answer);
		if (answer == "YES") {
			if ((*files).get(path).directory) {
				(*files).deldir(path);
			}
			else {
				(*files).delfile(path);
			}
		}
	}
	if (option == "write new data") {
		string val;
		val = Readfile(Findfile());

		if((*files).get(path).directory){
			(*files).adddir(path, val);
		}
		else {
			(*files).addfile(val, path);
		}
	}
	if (option == "extract") {
		if (!(*files).get(path).directory) {
			cout << "Find file to write" << endl;
			_getch();

			Writefile(
				Findfile(),
				(*files).get(path).payload
			);
		}
	}

	Writefile(sourcefile, (*files).code());
}
void Draw(Filesystem curdir, List<std::string> path, short start, short end, short adr) {
	cout << "Current directory: Main/" << path.str("/") << endl;
	cout << "------------------" << endl;
	if (curdir.containment.length > 0) {
		for (short q = start; q < end; q++) {
			if (q == adr) {
				cout << Pointer;
			}
			else {
				cout << Repeat(' ', strlen(Pointer));
			}
			cout << curdir.containment.get(q).name;
			if (curdir.containment.get(q).directory) {
				cout << '{' << curdir.containment.get(q).containment.length << '}';
			}
			cout << endl;
		}
	}
	else {
		cout << "(empty)" << endl;
		for (short q = 0; q < curdir.containment.length - 1; q++) {
			cout << endl;
		}
	}
	if (curdir.containment.length < WindowSize) {
		for (short q = 0; q < WindowSize - curdir.containment.length; q++) {
			cout << endl;
		}
	}
	cout << "------------------" << endl;
	cout << "To move press '" << Up << "' and '" << Down << "' buttons." << endl;
	cout << "To chose press '" << Chose << "' button." << endl;
	cout << "To use dir/file press '" << Use << "' button." << endl;
	cout << "To go back press '" << Back << "' button." << endl;
}



int main() {
	system("color F0");

	Filesystem files;
	Filesystem curdir;
	List<string> path;
	string sourcefile;

	cout << "Press 'm' to read manual" << endl;
	if (_getch() == 'm') {
		system("cls");

		cout << "This program is created to store a hidden filesystem inside a file." << endl;
		cout << "Filesystem - is just a directory. It can store file and other directories inside it." << endl;
		cout << "Each directory has a name, decription and containment (files/directories) inside it." << endl;
		cout << "Each file has a name and payload (information inside it)." << endl;
		cout << "To create a file, you must first save it in your OS." << endl;
		cout << "Than enter this program and use 'create new file' option and chose your file." << endl;
		cout << "To create a directory just use 'create new directory' option." << endl;
		cout << "To extract file back to windows use 'extract' option." << endl;
		cout << endl;
		cout << "WARNING:" << endl;
		cout << "Chosen files will be permanently rewritten." << endl;
		cout << "It will be IMPOSSIBLE to restore old data if you will chose file that you need." << endl;
		cout << endl;
		cout << "Other: Program is written by lazy programmer so it`s really uncomfortable to use it. (sorry, but finishing this program was incredibly boring so i left it 'raw')" << endl;
		cout << "Also it oftenly fails working, so be carefull." << endl;
		_getch();
	}
	system("cls");

	cout << "Press 'n' to create new filesystem, 'f' to find existing." << endl;
	char k;
	bool c = 1;
	while (c) {
		k = _getch();
		switch (k) {
		case 'n':{
			cout << "Find path to save your file" << endl;
			_getch();
			system("cls");

			sourcefile = Findfile();

			c = 0;
			break;
			}
		case 'f': {
			system("cls");

			sourcefile = Findfile();
			files.load(
				Readfile(
					sourcefile
				)
			);

			c = 0;
			break;
		}
		}
	}

	/*
	Example:
	files.addfile("Hello world", Split("A", '/'));
	files.adddir(Split("Directory0", '/'), "Something dir0");
	files.addfile("Hello world", Split("Directory0/B", '/'));
	files.adddir(Split("Directory0/Directory1", '/'), "Something dir1");
	files.addfile("Hello world", Split("Directory0/Directory1/C", '/'));
	files.addfile("Hello world", Split("Directory0/Directory1/D", '/'));
	files.addfile("Hello world", Split("Directory0/Directory1/E", '/'));
	files.addfile("Hello world", Split("Directory0/Directory1/F", '/'));
	files.adddir(Split("Directory0/Directory1/Directory2", '/'), "Something dir2");
	files.adddir(Split("Directory0/Directory1/Directory2/Directory3", '/'), "Something dir3");
	files.adddir(Split("Directory0/Directory1/Directory2/Directory5", '/'), "Something dir5");
	files.delfile(Split("Directory0/Directory1/E", '/'));
	files.deldir(Split("Directory0/Directory1/Directory2/Directory3", '/'));
	files.addsys(0, Split("Directory0/Directory1/G", '/'), "Hello world");
	*/

	char key;
	short adr, start, end;
	adr = 0;
	start = 0;
	end = min(WindowSize, curdir.containment.length) + 2;
	while (1) {
		curdir = files.get(path);

		if (adr < start) {
			if (key == Up) {
				start = adr;
				end = start + WindowSize;
			}
		}
		if (adr >= end) {
			if (key == Down) {
				end = adr + 1;
				start = end - WindowSize;
			}
		}
		end = min(end, curdir.containment.length);

		system("cls");
		Draw(curdir, path, start, end, adr);

		key = _getch();
		switch (key) {
		case Up:
		{
			if (adr > 0) {
				adr -= 1;
			}
			break;
		}
		case Down:
		{
			if (adr < curdir.containment.length - 1) {
				adr += 1;
			}
			break;
		}
		case Chose:
		{
			if (curdir.containment.length > 0) {
				if (curdir.containment.get(adr).directory) {
					path.append(curdir.containment.get(adr).name);
					adr = 0;
					start = 0;
					end = WindowSize;
				}
			}

			break;
		}
		case Back:
		{
			start = 0;
			end = WindowSize;
			adr = 0;

			if (path.length > 0) {
				path.del(path.length - 1);
			}
			break;
		}
		case Use:
		{
			if (curdir.containment.length > 0) {
				path.append(curdir.containment.get(adr).name);

				system("cls");
				Change(&files, path, sourcefile);

				path.del(path.length - 1);
			}
			else {
				system("cls");
				Change(&files, path, sourcefile);
			}

			break;
		}
		}
	}

	return 0;
}