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


	//not support safe base64 !
	static binstream encode(binstream& mem, bool safe = false);
	static binstream decode(binstream& mem, bool safe = false);

private:




};


_NAP_END