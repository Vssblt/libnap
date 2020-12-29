#pragma once
#include "nap_common.h"
#include "binstream.h"
_NAP_BEGIN

class Hex {
public:
	Hex();
	~Hex();

	//将二进制字符串转为十六进制形式字符串
	binstream toHex(binstream& mem, bool up = false);

	//将十六进制字符串转为二进制
	//如果奇数长度，则认为最后一位为高位
	binstream toBinary(binstream& mem);

private:
	void to_hex(const void*, size_t, bool up);
	void to_bin(const void*, size_t);

	uint8_t* buffer;
	size_t length;
};

class Base64 {
public:

	static binstream encode(binstream& mem, bool safe = false);
	static binstream decode(binstream& mem, bool safe = false);

private:

	static void enblock(uint8_t* str, uint8_t* base, const char*);
	static void deblock(uint8_t* str, uint8_t* base, const char*);

};


_NAP_END