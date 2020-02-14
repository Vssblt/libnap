#pragma once
#include "nap_common.h"
#include "binstream.h"
_NAP_BEGIN

typedef const char* Key;
typedef uint8_t* Matrix4x4;

enum AesPadding {
	PKCS5, //��������
	ISO10126,//���һλ�����������������
	Zeros,  //0������ʱ�����Զ�ȥ��
	PKCS7
};

enum AesType {
	ECB,
	CBC
};




class Aes {
public:

	static Aes cipher(Key key,AesPadding, AesType,Key iv=nullptr);

	binstream encode(const char* plaintext, int len);
	bool decode(const char* ciphertext,int len,binstream&);

	~Aes();
	Aes(Aes&&);
private:
	////ECBģʽ�ļ��ܽ���
	//void ecb_encode();
	//void ecb_decode(uint8_t* matrix4x4);

	//16byte�ӽ���(����ӽ���)
	void _16encode(Matrix4x4 matrix4x4);
	void _16decode(Matrix4x4 matrix4x4);

	//������Կ
	void setKey(uint8_t key[16]);

	//����ת��
	void char2matrix4x4(Matrix4x4 out,const uint8_t* in);
	void matrix4x42char(uint8_t* out,const Matrix4x4 in);

	//��Կ��չ
	void keyexpan(uint8_t* key);

	//�ֽ��滻
	void subbytes(uint8_t* matrix4x4);
	void invsubbytes(uint8_t* matrix4x4);

	//����λ
	void leftshift(uint8_t* matrix1x4, int b);
	void rightshift(uint8_t* matrix1x4, int b);
	void shiftrows(uint8_t* matrix4x4);
	void invshiftrows(uint8_t* matrix4x4);

	//�л���
	uint8_t g_num(uint8_t u, uint8_t v);
	void mixcolumns(uint8_t* matrix4x4);
	void invmixcolumns(uint8_t* matrix4x4);
	
	//����Կ��
	void addroundkey(uint8_t* matrix4x4, uint32_t* _4rkey);

	uint8_t* iv;
	uint32_t keyse[44]; //������Կ����
	uint32_t keysd[44]; //������Կ����
	const AesPadding padd;
	const AesType type;	

	Aes(AesPadding, AesType);

};


















_NAP_END