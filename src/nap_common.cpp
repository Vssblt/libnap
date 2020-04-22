#include "nap_common.h"

_NAP_BEGIN

uint32_t timestamp() {
	time_t t = time(NULL);
	uint32_t ii = (uint32_t)time(&t);
	return ii;
}

void msleep(uint64_t millisecond) {
#ifdef LINUX
	//if ((millisecond * 1000) > UINT32_MAX) {
	//	sleep(millisecond / 1000)
	//}else {
	usleep(millisecond * 1000);
	//}
#endif

#ifdef WINDOWS
	Sleep(millisecond);
#endif
}


_NAP_END