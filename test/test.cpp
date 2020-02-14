#include "../src/libnap.h"
#include <conio.h>
#include <time.h>
#include "aes.h"

using namespace std;
using namespace nap;

////Link Library
//#ifdef WINDOWS
//#ifdef _WIN64
//	#ifdef _DEBUG
//			//#pragma comment (lib,"../Win/x64/Debug/libw.lib")
//			#pragma comment (lib,"cryptlib_debug_x64.lib")
//		#else
//			//#pragma comment (lib,"../Win/x64/Release/libw.lib")
//			#pragma comment (lib,"cryptlib_release_x64.lib")
//	#endif
//#else
//	#ifdef _DEBUG
//			//#pragma comment (lib,"../Win/Debug/libw.lib")
//			#pragma comment (lib,"cryptlib_debug_x86")
//		#else
//			//#pragma comment (lib,"../Win/Release/libw.lib")
//			#pragma comment (lib,"cryptlib_release_x86")
//	#endif
//#endif
//#endif


/*


跨平台模块的实现依赖 nap_common.h
网络
内存池(gc)
线程池
反射
日志
反射
Mysql API
简单加密库
*/


bool aes_test() {
	Aes aes = Aes::cipher("1\x12V4567890123456", AesPadding::PKCS5, ECB);
	for (int i = 0; i < 1000; i++) {
		const char* p = "aabbccddwweerrttnddwweerrttnmp";
		auto c = aes.encode(p, strlen(p));
		binstream b;
		aes.decode((const char*)c.ustr(), c.len(), b);
		assert(b == p);
	}

	Aes aes2 = Aes::cipher("@\x12V45678901234TEST", AesPadding::PKCS5, CBC);
	for (int i = 0; i < 1000; i++) {
		const char* p2 = "this is a plaintext....";
		auto c2 = aes2.encode(p2, strlen(p2));
		binstream b2;
		aes2.decode((const char*)c2.ustr(), c2.len(), b2);
		assert(b2 == p2);
	}


	return true;
}

#define START startTime = clock();{
#define END }endTime = clock();
#define PRINTT(MNAME,C) cout << MNAME<<" "<< (double)clock() / CLOCKS_PER_SEC / C<< "ps" << endl;

int main() {

	clock_t startTime, endTime;

	START
		assert(aes_test());
	END
	PRINTT("Aes",2000)

	_getch();

	return 0;
}