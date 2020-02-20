#pragma once
#include "nap_common.h"
#include "binstream.h"
_NAP_BEGIN

uint32_t mhash(const char* str, uint32_t len);



class SHA256 {

public:

	SHA256();

	binstream calculator();
	void add(const char* data, uint32_t len);

private:

	uint32_t sha256[8] = { 0 };
	uint8_t sha256_buffer[64] = { 0 };
	uint32_t sha256_buffer_len = 0;
	uint64_t calByte = 0;

	//计算长度为512bit的sha256值
	void calSHA256();

};




class Hex{
public:
	Hex();
	~Hex();

	uint8_t* hex(const void* memory, int& len);
	binstream hex(binstream& mem);

private:

	uint8_t* buffer;
	uint32_t length;
};


_NAP_END