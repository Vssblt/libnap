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

����
�ڴ��(gc)
�̳߳�
����
��־
����
Mysql API
*/


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

int main() {

	assert(binstream_test());

	try {
		Unit unit64("BASE64");
		unit64.test([](binstream* p, int n)->bool {
			assert(n == 3);
			bool safe;
			if (p[1] == "safe")
				safe = true;
			else
				safe = false;
			if (!(p[2] == Base64::encode(p[0], safe)))return false;
			if (!(p[0] == Base64::decode(p[2], safe)))return false;
			return true;
		});
		unit64.print();

		Unit unit256("SHA256");
		unit256.test([](binstream* p, int n)->bool {
			assert(n == 3);
			Hex hex;
			SHA256 S;
			S.add((const char*)p[0].str(), p[0].size());
			S.add((const char*)p[1].str(), p[1].size());
			binstream sha256 = S.calculator();
			binstream sha256_hex = hex.toHex(sha256, false);
			return (sha256_hex == p[2]);
		});
		unit256.print();

		Unit unitaescbc("AES-128-CBC");
		unitaescbc.test([](binstream* p, int n)->bool {
			assert(n == 4);
			Hex hex;
			Aes aes = Aes::cipher((char*)p[1].str(),
				AesPadding::PKCS5, AesType::CBC, (char*)p[2].str());
			//���ܹ���
			binstream res = aes.encode((const char*)p[0].str(), p[0].size());
			binstream res_hex = hex.toHex(res);
			if (!(res_hex == p[3])) return false;
			//���ܹ���
			binstream res2;
			binstream cipher = hex.toBinary(p[3]);
			aes.decode((const char*)cipher.str(), cipher.size(), res2);
			if (!(res2 == p[0])) return false;
			return true;
		});
		unitaescbc.print();

		Unit unitaesecb("AES-128-ECB");
		unitaesecb.test([](binstream* p, int n)->bool {
			assert(n == 3);
			Hex hex;
			Aes aes = Aes::cipher((char*)p[1].str(),
				AesPadding::PKCS5, AesType::ECB);
			//���ܹ���
			binstream res = aes.encode((const char*)p[0].str(), p[0].size());
			binstream res_hex = hex.toHex(res);
			if (!(res_hex == p[2])) return false;
			//���ܹ���
			binstream res2;
			binstream cipher = hex.toBinary(p[2]);
			aes.decode((const char*)cipher.str(), cipher.size(), res2);
			if (!(res2 == p[0])) return false;
			return true;
		});
		unitaesecb.print();
	}
	catch (const char* e) {
		cout << "TEST-EXCEPTION: " << e<<endl;
	}


#ifndef LINUX
	auto no_use = _getch();
#endif

	return 0;
}
