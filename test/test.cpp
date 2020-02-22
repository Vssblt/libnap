#include "libnap.h"
#ifdef  WINDOWS
	#include <conio.h>
#endif //  WINDOWS

#include <time.h>
#include <fstream>


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



	//ECB
	Aes aes = Aes::cipher("12V4567890123456", AesPadding::PKCS5, AesType::ECB);
	Hex hex;
	for (int i = 0; i < 100; i++) {
		const char* p = "aabbccddwweerrttnddwweerrttnmp";
		binstream c = aes.encode(p, strlen(p));

		binstream c_hex = hex.toHex(c);
		binstream r = "34c5b033c16f1f3d45f98783a9422418a039b48a4f6ff28a46d16991e1c29820";
		assert(r == c_hex);
		
		binstream b;
		aes.decode((const char*)c.str(), c.size(), b);
		assert(b == p);
	}

	

	//CBC
	Aes aes2 = Aes::cipher("1111111111111111", 
		AesPadding::PKCS5, AesType::CBC,"1111111111111111");
	for (int i = 0; i < 100; i++) {
		const char* p2 = "this is a plaintext....";
		auto c2 = aes2.encode(p2, strlen(p2));

		binstream c2_hex = hex.toHex(c2);
		binstream r2 = "c2b1bc7811375043abce3b41d21c2988cd1bf646ab85eb91de4ed6dbfa706f0c";
		assert(r2 == c2_hex);

		binstream b2;
		aes2.decode((const char*)c2.str(), c2.size(), b2);
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
	"A4F0DAC980948344E73D2F9E0AA165032878FA7FE64D6A020E168E3E55EB141A"
	};

	Hex hex;
	for (int i = 0; i < 100; i++) {
		for (int i = 1; i < 2; i++) {
			SHA256 S;
			S.add(str[i], strlen(str[i]));
			binstream sha256 = S.calculator();

			binstream sha256_hex = hex.toHex(sha256,true);

			if (strncmp((char*)sha256_hex.str(), s256[i], 64) != 0)
				return false;
		}
		SHA256 S;
		S.add(str[0], strlen(str[0]));
		S.add(str[1], strlen(str[1]));
		binstream sha256 = S.calculator();
		binstream sha256_hex = hex.toHex(sha256,true);
		if (strncmp((char*)sha256_hex.str(), s256[2], 64) != 0)
			return false;
		
	}

	return true;
}
bool binstream_test() {
	for (int i = 0; i < 1000; i++) {

		binstream str;
		binstream str_2(100);
		str = "Whatever is worth doing is worth doing well.";
		//cout << str;
		str = std::string("In love folly is always sweet.");
		//cout << endl<<str<<endl;

		str.reserve(100);
		str.append("hello world", 11);
		str.append(string("\nThis is the NapStream"));
		//cout << str.toStdString() <<endl;
		const char* teststr = "hello world\nThis is the NapStream";

		//operate ==
		assert(str == teststr);
		assert(str == str.toStdString());

		//construct
		assert(str == binstream(teststr));
		assert(str == binstream(string(teststr)));
		assert(str == binstream(teststr, strlen(teststr)));
		assert(str == binstream(std::move(binstream(str))));

		assert(str == str);
		assert(memcmp(str.str(), teststr, strlen(teststr)) == 0);

		//operate=
		str_2 = str;
		assert(str_2.toStdString() == str.toStdString());


		str_2.fill('-', 10);
		//cout << str_2 << endl;
		for (int i = 9; i > 0; i--) {
			str_2.resize(i);
			//cout << str_2 << endl;
		}

	}
	return true;
}


#define START startTime = clock();{
#define END }endTime = clock();
#define PRINTT(MNAME,C) cout << MNAME<<" "<< (double)clock() / CLOCKS_PER_SEC / C<< "ps" << endl;

void use(clock_t){}


int main() {

	clock_t startTime=0, endTime=0;
	use(startTime);
	use(endTime);

	START
		assert(aes_test());
	END
	PRINTT("Aes",200)

	START
	assert(sha256_test());
	END
	PRINTT("SHA256",200)

	START
	assert(binstream_test());
	END
	PRINTT("Binstream", 1000)

	//binstream b("ABCDE");
	//auto i = Base64::encode(b);
	//cout << Base64::decode(i);


#ifndef LINUX
	auto no_use = _getch();
#endif

	return 0;
}


//ba7816bf 8f01cfea 414140de5dae2223b00361a396177a9cb410ff61f20015ad abc


//49 23 37 25 59 f5 f4 e2 57 f8 65 ae 4c 1e 72 4e