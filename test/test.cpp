#include "libnap.h"
#ifdef  WINDOWS
	#include <conio.h>
#endif //  WINDOWS
#include "unit.h"
#include <time.h>
#include <fstream>

using namespace std;
using namespace nap;

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

//
//bool aes_test() {
//
//	//ECB
//	Aes aes = Aes::cipher("12V4567890123456", AesPadding::PKCS5, AesType::ECB);
//	Hex hex;
//	for (int i = 0; i < 100; i++) {
//		const char* p = "aabbccddwweerrttnddwweerrttnmp";
//		binstream c = aes.encode(p, strlen(p));
//
//		binstream c_hex = hex.toHex(c);
//		binstream r = "34c5b033c16f1f3d45f98783a9422418a039b48a4f6ff28a46d16991e1c29820";
//		assert(r == c_hex);
//		
//		binstream b;
//		aes.decode((const char*)c.str(), c.size(), b);
//		assert(b == p);
//	}
//
//
//	//CBC
//	Aes aes2 = Aes::cipher("1111111111111111", 
//		AesPadding::PKCS5, AesType::CBC,"1111111111111111");
//	for (int i = 0; i < 100; i++) {
//		const char* p2 = "this is a plaintext....";
//		auto c2 = aes2.encode(p2, strlen(p2));
//
//		binstream c2_hex = hex.toHex(c2);
//		binstream r2 = "c2b1bc7811375043abce3b41d21c2988cd1bf646ab85eb91de4ed6dbfa706f0c";
//		assert(r2 == c2_hex);
//
//		binstream b2;
//		aes2.decode((const char*)c2.str(), c2.size(), b2);
//		assert(b2 == p2);
//	}
//
//	return true;
//}
//bool sha256_test() {
//
//	const char str[][512] = { 
//		"helloworld",
//		"textlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtextlongtext",
//	};
//	const char s256[][65] = {
//	"936A185CAAA266BB9CBE981E9E05CB78CD732B0B3280EB944412BB6F8F8F07AF",
//	"37F8615468B36A2DBA75D2C32E3FD8F552A083E051310FE42EA199281C0D15D4",
//	"A4F0DAC980948344E73D2F9E0AA165032878FA7FE64D6A020E168E3E55EB141A"
//	};
//
//	Hex hex;
//	for (int i = 0; i < 100; i++) {
//		for (int i = 1; i < 2; i++) {
//			SHA256 S;
//			S.add(str[i], strlen(str[i]));
//			binstream sha256 = S.calculator();
//
//			binstream sha256_hex = hex.toHex(sha256,true);
//
//			if (strncmp((char*)sha256_hex.str(), s256[i], 64) != 0)
//				return false;
//		}
//		SHA256 S;
//		S.add(str[0], strlen(str[0]));
//		S.add(str[1], strlen(str[1]));
//		binstream sha256 = S.calculator();
//		binstream sha256_hex = hex.toHex(sha256,true);
//		if (strncmp((char*)sha256_hex.str(), s256[2], 64) != 0)
//			return false;
//		
//	}
//
//	return true;
//}

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
		assert(!(str != teststr));
		assert(!(str != str.toStdString()));

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

		//operate +=
		str.reserve(1);
		str = "test";
		str += str;
		str += "-test";
		str += string("test");
		assert(str == "testtest-testtest");


		str_2.fill('-', 10);
		//cout << str_2 << endl;
		for (int i = 9; i > 0; i--) {
			str_2.resize(i);
			//cout << str_2 << endl;
		}

	}
	return true;
}


//#define START startTime = clock();{
//#define END }endTime = clock();
//#define PRINTT(MNAME,C) cout << MNAME<<" "<< (double)clock() / CLOCKS_PER_SEC / C<< "ps" << endl;

//void use(clock_t){}

int main() {

	assert(binstream_test());

	//try {
	//	Unit unit64("BASE64");
	//	unit64.test([](binstream* p, int n)->bool {
	//		assert(n == 3);
	//		if (!(p[2] == Base64::encode(p[0])))return false;
	//		if (!(p[0] == Base64::decode(p[2])))return false;
	//		return true;
	//		});
	//	unit64.print();

	//	Unit unit256("SHA256");
	//	unit256.test([](binstream* p, int n)->bool {
	//		assert(n == 3);
	//		Hex hex;
	//		SHA256 S;
	//		S.add((const char*)p[0].str(), p[0].size());
	//		S.add((const char*)p[1].str(), p[1].size());
	//		binstream sha256 = S.calculator();
	//		binstream sha256_hex = hex.toHex(sha256, false);
	//		return (sha256_hex == p[2]);
	//		});
	//	unit256.print();

	//	Unit unitaescbc("AES-128-CBC");
	//	unitaescbc.test([](binstream* p, int n)->bool {
	//		assert(n == 4);
	//		Hex hex;
	//		Aes aes = Aes::cipher((char*)p[1].str(),
	//			AesPadding::PKCS5, AesType::CBC, (char*)p[2].str());
	//		//加密过程
	//		binstream res = aes.encode((const char*)p[0].str(), p[0].size());
	//		binstream res_hex = hex.toHex(res);
	//		if (!(res_hex == p[3])) return false;
	//		//解密过程
	//		binstream res2;
	//		binstream cipher = hex.toBinary(p[3]);
	//		aes.decode((const char*)cipher.str(), cipher.size(), res2);
	//		if (!(res2 == p[0])) return false;
	//		return true;
	//		});
	//	unitaescbc.print();

	//	Unit unitaesecb("AES-128-ECB");
	//	unitaesecb.test([](binstream* p, int n)->bool {
	//		assert(n == 3);
	//		Hex hex;
	//		Aes aes = Aes::cipher((char*)p[1].str(),
	//			AesPadding::PKCS5, AesType::ECB);
	//		//加密过程
	//		binstream res = aes.encode((const char*)p[0].str(), p[0].size());
	//		binstream res_hex = hex.toHex(res);
	//		if (!(res_hex == p[2])) return false;
	//		//解密过程
	//		binstream res2;
	//		binstream cipher = hex.toBinary(p[2]);
	//		aes.decode((const char*)cipher.str(), cipher.size(), res2);
	//		if (!(res2 == p[0])) return false;
	//		return true;
	//		});
	//	unitaesecb.print();
	//}
	//catch (const char* e) {
	//	cout << "TEST-EXCEPTION: " << e<<endl;
	//}

	//clock_t startTime=0, endTime=0;
	//use(startTime);
	//use(endTime);

	//START
	//	assert(aes_test());
	//END
	//PRINTT("Aes",200)

	//START
	//assert(sha256_test());
	//END
	//PRINTT("SHA256",200)

	//START
	//assert(binstream_test());
	//END
	//PRINTT("Binstream", 1000)

	//binstream b("ABCDE");
	//auto i = Base64::encode(b);
	//cout << Base64::decode(i);


#ifndef LINUX
	auto no_use = _getch();
#endif

	return 0;
}
