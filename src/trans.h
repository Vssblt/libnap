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
	void to_hex(const void*, int, bool up);
	void to_bin(const void*, int);

	uint8_t* buffer;
	uint32_t length;
};

class Base64 {
public:


	//not support safe base64 !
	static binstream encode(binstream& mem, bool safe = false);
	static binstream decode(binstream& mem, bool safe = false);

private:




};


_NAP_END