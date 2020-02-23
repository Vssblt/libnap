#pragma once
#include "nap_common.h"
#include "binstream.h"
_NAP_BEGIN

class Hex {
public:
	Hex();
	~Hex();

	//���������ַ���תΪʮ��������ʽ�ַ���
	binstream toHex(binstream& mem, bool up = false);

	//��ʮ�������ַ���תΪ������
	//����������ȣ�����Ϊ���һλΪ��λ
	binstream toBinary(binstream& mem);

private:
	void to_hex(const void*, int, bool up);
	void to_bin(const void*, int);

	uint8_t* buffer;
	uint32_t length;
};

class Base64 {
public:
	//base ֧�ְ�ȫģʽ
	//	binstream ֧�� []
	//		base64�������� 

	//	aes֧�ִ�������

	//	д�ĵ�


	//not support safe base64 !
	static binstream encode(binstream& mem, bool safe = false);
	static binstream decode(binstream& mem, bool safe = false);

private:

	static void enblock(uint8_t* str, uint8_t* base, const char*);
	static void deblock(uint8_t* str, uint8_t* base, const char*);


};


_NAP_END