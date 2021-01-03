#pragma once
#include "nap_common.h"
_NAP_BEGIN

class binstream {
public:


	void swap(binstream& b);//交换两个字符串
	static binstream shift(char** buffer, size_t len); //将字符串转移到binstream

	template <class T>
	T to(); //强制转换

	template <class T>
	static binstream from(const T num);  //T类型转字符串


	inline size_t size() const { return length; }
	inline size_t cap() const { return capacity; }
	
	binstream() {};
	~binstream();
	binstream(const void* c, size_t len)noexcept;
	binstream(const char* c)noexcept;
	binstream(const std::string& str)noexcept;
	explicit binstream(size_t len)noexcept { _recap(len); }
	binstream(const binstream& old) noexcept;
	binstream(binstream&& old) noexcept;

	friend binstream operator+(const char* s1, binstream& s2);
	friend binstream operator+(binstream& s1, const char* s2);
	friend binstream operator+(const std::string& s1, binstream& s2);
	friend binstream operator+(binstream& s1, const std::string& s2);
	binstream operator+(const binstream& s);

	void operator+=(const binstream& o) {append(o);}
	void operator+=(const char* o) { append(o, strlen(o)); }
	void operator+=(const std::string& o) { append(o); }
	void operator+=(const char c) {append(&c,1);}
	void append(const void* c, size_t len);
	void append(const std::string& str);
	void append(const binstream& str);

	bool operator==(const binstream& o);
	bool operator==(const void* str);
	bool operator==(const std::string& str);
	inline bool operator!=(const binstream& o) {return !(*this == o);}
	inline bool operator!=(const void* str) {return !(*this == str);}
	inline bool operator!=(const std::string& str) {return !(*this == str);}

	binstream& operator=(const binstream& old) noexcept;
	binstream& operator=(const std::string& str) noexcept;
	binstream& operator=(const char* str) noexcept;


	friend std::ostream& operator<<(std::ostream& out, const binstream& b);
	friend std::istream& operator>>(std::istream& is, binstream& b);

	
	void fill(uint8_t c, size_t len); //填充指定长度字符，会删除以前字符串内容

	//预留指定长度空间，会删除以前字符串内容
	//执行完后length为0，capacity大于等于参数len
	void reserve(size_t len);

	//更改空间长度，len如果小于当前字符串长度会截断
	//执行完后length为len，capacity大于等于参数len
	void resize(size_t len);


	//将长度设置为0
	void clean() noexcept {length = 0;}

	//获取指定位置字符，越界抛出异常
	uint8_t at(size_t pos);

	//获取指定位置字符，不进行越界检查
	uint8_t& operator[](size_t pos) noexcept;

	//获取指定位置字符，越界会回溯
	uint8_t& operator()(size_t pos) noexcept;

	//返回一个标准c++ string
	std::string toStdString();

	//获取不带0结尾的字符串
	inline uint8_t* str() const{return content;};

	//获取第一个字符的地址
	inline uint8_t* begin() { return (uint8_t*)content;}

	//获取最后一个字符的下一个地址
	inline uint8_t* end(){return (uint8_t*)content+length;}

	//返回长度是否为0
	inline bool empty() noexcept { return (length == 0); }

protected:

	//在当前字符串后追加字符串
	//追加长度为0的字符串将不会改变原字符串
	void _append(const uint8_t* con, size_t _length);

	//设置一串字符串，将覆盖之前的字符串
	//设置长度为0的字符串会使字符串长度为0
	void _set(const uint8_t* con, size_t _length);

	//重新分配空间，大小为cap
	void _recap(size_t _cap);

	//重新分配空间，大小为cap,且保留以前空间内容
	void _recap_hold(size_t _cap);

private:
	uint8_t* content = nullptr;
	size_t capacity = 0; //content length
	size_t length = 0;   //string length
};

template<class T>
inline binstream binstream::from(const T n) {
	binstream bins;
	std::stringstream strs;
	strs << n;
	strs >> bins;
	return binstream(bins);
}

template<class T>
inline T binstream::to(){
	//does not support boolean
	T x{};
	std::stringstream strs;
	strs << *this;
	strs >> x;
	return x;
}

template<>
inline std::string binstream::to() {
	return this->toStdString();
}

_NAP_END