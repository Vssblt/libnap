#include "nap_common.h"

_NAP_BEGIN

uint32_t timestamp() {
	time_t t = time(NULL);
	uint32_t ii = (uint32_t)time(&t);
	return ii;
}



_NAP_END