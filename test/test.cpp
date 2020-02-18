#include "../src/libnap.h"
#ifdef  WINDOWS
	#include <conio.h>
#endif //  WINDOWS

#include <time.h>
#include "aes.h"
#include "hash.h"

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
	Aes aes = Aes::cipher("1\x12V4567890123456", AesPadding::PKCS5, AesType::ECB);
	for (int i = 0; i < 100; i++) {
		const char* p = "aabbccddwweerrttnddwweerrttnmp";
		auto c = aes.encode(p, strlen(p));
		binstream b;
		aes.decode((const char*)c.ustr(), c.len(), b);
		assert(b == p);
	}

	Aes aes2 = Aes::cipher("@\x12V45678901234TEST", AesPadding::PKCS5, AesType::CBC);
	for (int i = 0; i < 100; i++) {
		const char* p2 = "this is a plaintext....";
		auto c2 = aes2.encode(p2, strlen(p2));
		binstream b2;
		aes2.decode((const char*)c2.ustr(), c2.len(), b2);
		assert(b2 == p2);
	}


	return true;
}

bool sha256_test() {

	const char str[][512] = { 
		"helloworld",
		"textlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtext",
	};
	const char s256[][65] = {
	"936A185CAAA266BB9CBE981E9E05CB78CD732B0B3280EB944412BB6F8F8F07AF",
	"37F8615468B36A2DBA75D2C32E3FD8F552A083E051310FE42EA199281C0D15D4",
	};

	Hex hex;
	for (int i = 0; i < 100; i++) {
		for (int i = 1; i < 2; i++) {
			SHA256 S;
			S.add(str[i], strlen(str[i]));
			uint8_t* sha256 = S.calculator();

			int len = 32;
			uint8_t* bufer = hex.hex(sha256, len);

			if (strncmp((char*)bufer, s256[i], 64) != 0)
				return false;
		}
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
	PRINTT("Aes",200)

	START
	assert(sha256_test());
	END
	PRINTT("SHA256",200)

#ifndef LINUX
	auto no_use = _getch();
#endif

	return 0;
}


//ba7816bf 8f01cfea 414140de5dae2223b00361a396177a9cb410ff61f20015ad abc


//49 23 37 25 59 f5 f4 e2 57 f8 65 ae 4c 1e 72 4e