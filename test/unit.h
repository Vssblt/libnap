#pragma once
#include "libnap.h"
#include <fstream>

using namespace std;
using namespace nap;

class Unit {
public:

	Unit(const char*);
	~Unit();

	typedef std::function <bool(binstream * p, int n)> callback;
	void test(callback cb);
	void print();

private:

	void init();

	ifstream ifs;
	int num; //测试总数量
	int line;//单个测试案例行数
	int line_max_char; //一行最多的字符长度
	const char* name;

	//result
	vector<int> nopass;
	int pass = 0;

	binstream** unit;
};