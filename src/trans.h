#pragma once
#include "nap_common.h"
#include "binstream.h"
_NAP_BEGIN

class Hex {
public:

	//Convert binary to hex form string
	static binstream encode(binstream& mem, bool capital= false);

	//Convert hex string to binary
	//If the length is odd, the last bit is considered as the high bit
	static binstream decode(binstream& mem);

};

class Base64 {
public:

	//Convert binary to base64 form string
	static binstream encode(binstream& mem, bool safe = false);

	//Convert base64 to binary
	static binstream decode(binstream& mem, bool safe = false);

private:

	static void enblock(uint8_t* str, uint8_t* base, const char*);
	static void deblock(uint8_t* str, uint8_t* base, const char*);

};


_NAP_END