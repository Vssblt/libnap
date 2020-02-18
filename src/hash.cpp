#include "hash.h"
_NAP_BEGIN

uint32_t mhash(const char* str, uint32_t len){
	uint32_t hash = 0;
	uint32_t x = 0;
	uint32_t i = 0;
	for (i = 0; i < len; str++, i++) {
		hash = (hash << 4) + (*str);
		if ((x = hash & 0xF0000000L) != 0) {
			hash ^= (x >> 24);
		}
		hash &= ~x;
	}
	return hash;
}


_NAP_END

