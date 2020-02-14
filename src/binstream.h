#pragma once
#include <stdint.h>
#include <algorithm>
#include <string>
#include "nap_common.h"

/*
all function need support 
	char*
	uchar*
	string
	self

functions:
	set
	get
	append
	construct

	缺少很多函数 例如 renew reserve

*/
_NAP_BEGIN
class binstream
{
public:

	binstream();
	binstream(const char* content,uint32_t len)noexcept;
	binstream(const uint8_t* content, uint32_t len)noexcept;
	binstream(const std::pair<const char*, uint32_t>&)noexcept;
	binstream(const std::pair<const uint8_t*,uint32_t>&)noexcept;
	binstream(const std::string&)noexcept;
	binstream(const char*)noexcept;
	binstream(uint32_t len)noexcept;
	binstream(binstream&) noexcept;
	binstream(binstream&&) noexcept;
	binstream& operator=(const binstream&) noexcept;
	binstream& operator=(const std::string&) noexcept;
	binstream& operator=(const char*) noexcept;
	binstream& operator=(uint8_t*) noexcept;

	~binstream();

	bool operator==(const binstream&);
	bool operator==(const char*);
	bool operator==(const int8_t*);
	bool operator==(const std::string&);

	void append(const binstream&) noexcept;
	void append(const std::string&) noexcept;
	void append(const char*, uint32_t) noexcept;
	void append(const uint8_t*, uint32_t) noexcept;
	void append(const char*) noexcept;
	void append(const char) noexcept;

	binstream& operator+=(const char*)noexcept;
	binstream& operator+=(const uint8_t*)noexcept;
	binstream& operator+=(const binstream&)noexcept;
	binstream& operator+=(const std::string&)noexcept;

	binstream& operator<<(const char*);
	binstream& operator<<(const uint8_t*);
	binstream& operator<<(const binstream&);
	binstream& operator<<(const std::string&);

	friend std::ostream& operator<<(std::ostream&,const binstream&);
	friend std::istream& operator>>(std::istream&, binstream&);

//字符串操作
	binstream substr(int pos, int length);


//getter

	std::string toStdString();
	std::pair<const uint8_t*, uint32_t> toPair();
	
	/*
		获取指定字符，如果越界则返回 0
	*/
	uint8_t& operator[](uint32_t) noexcept;

	/*
		获取指定字符，如果越界访问会抛出异常
	*/
	uint8_t at(uint32_t);

	inline uint32_t len() const { return length; };
	inline const char* str() const{ return (const char*)content; };
	inline uint8_t* ustr() const { return content; };


//foreach support
	uint8_t* begin() { return content; }
	uint8_t* end(){return content+length;}

protected:
	//操作字符串

	void appendcontent(const uint8_t*, uint32_t length);
	void setcontent(const uint8_t*, uint32_t length);
	void setcontent(const char*, uint32_t length);
	void renew(uint32_t capacity); //重新初始化内容大小


private:

	uint8_t* content = nullptr;
	uint32_t capacity = 0; //content length
	uint32_t length = 0;   //string length

};

_NAP_END