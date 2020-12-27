#include  "unit.h"
#include  <sstream>

Unit::Unit(const char* _m):name(_m){
	stringstream file;
#ifdef WINDOWS
	file << "../test/";
#endif	

#ifdef LINUX
	file << "./test/";
#endif	

	file << _m << ".test.txt";
	ifs.open(file.str().c_str(), ios::in);
	if (!ifs.is_open()) {
		throw "File open fail";
	}
	char buffer[20];
	ifs.getline(buffer, 20);
	if (3 != sscanf(buffer, "%d %d %d", &num, &line, &line_max_char))
		throw "Wrong format";

	if (num < 1 || line < 1)
		throw "Data exception";

	unit = new binstream * [num];
	init();
}

Unit::~Unit(){
	for (int i = 0; i < num; i++) {
		delete[] unit[i];
	}
	delete[] unit;
}

void Unit::test(callback cb){
	for (int i = 0; i < num; i++) {
		bool ret = cb(unit[i], line);
		if (ret)
			pass++;
		else
			nopass.push_back(i + 1);
	}
}

void Unit::print(){
	if (pass == num) {
		cout << "Module " << name << " Test passed ("<<num<<")" << endl;
	}
	else {
		cout << "Module " << name << " Test failed" << endl;
		for (int& n : nopass)
			cout << "\t-Failed item: " << n << endl;
	}
}

void Unit::init(){
	char* buffer = new char[line_max_char];

	for (int i = 0; i < num; i++) {
		unit[i] = new binstream[line];
		for (int n = 0; n < line; n++) {
			memset(buffer, 0, line_max_char);
			ifs.getline(buffer, line_max_char);
			int len = (int)ifs.gcount();
			if (len == 0) {
				cout << strlen(buffer) <<" -- !"<<endl;
				throw "Number of test cases or maximum length error : eof is true";
			}
				
			if (buffer[len - 1] == 0) len--;
			if (buffer[len - 1] == '\r'||
				buffer[len - 1] == '\n'
			) len--;

			unit[i][n].append(buffer, len);
		}
		ifs.getline(buffer, 10); //Ïû³ý¿ÕÐÐ
	}

	delete[]buffer;
}
