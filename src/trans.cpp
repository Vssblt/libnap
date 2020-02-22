#include "trans.h"
_NAP_BEGIN

///////----------------hex
//二进制转为可读十六进制
#define HEX_TRANS(x,up) (((x)>0x09)?((x)+((up)?0x37:0x57)):((x)|0x30))
#define STR_TRANS(x) (((x)<0x40)?((x)-0x30):(((x)<0x47)?((x)-0x37):(((x)<0x67)?((x)-0x57):0x10)))


Hex::~Hex() {
	delete[] buffer;
}

Hex::Hex(){
	buffer = nullptr;
	length = 0;
}

binstream Hex::toHex(binstream& mem,bool up){
	uint32_t len = mem.size();
	to_hex(mem.str(), len,up);
	return (binstream::shift((char**)&buffer,length));
}

binstream Hex::toBinary(binstream& mem){
	uint32_t len = mem.size();
	to_bin(mem.str(), len);
	return (binstream::shift((char**)&buffer, length));
}

//trans to buffer
void Hex::to_hex(const void* m,int len, bool up) {
	if (buffer != nullptr) {
		delete buffer;
		buffer = nullptr;
	}
	length = len * 2LL;
	buffer = new uint8_t[length];
	
	const uint8_t* _m = (uint8_t*)m;
	for (int i = 0; i < len; i++) {
		buffer[i * 2] = HEX_TRANS(_m[i] >> 4,up);
		buffer[i * 2 + 1] = HEX_TRANS(_m[i] & 0x0F,up);
	}
}

void Hex::to_bin(const void* m, int len){
	if (buffer != nullptr) {
		delete buffer;
		buffer = nullptr;
	}

	bool odd = !(len % 2 == 0);
	length = len / 2;
	if (odd)
		length++;
	buffer = new uint8_t[length];

	const uint8_t* _m = (uint8_t*)m;
	for (int i = 0; i < length; i++) {
		buffer[i] = (STR_TRANS(_m[i * 2]))<< 4;
		if (!odd || (i*2+1<len))
			buffer[i] += STR_TRANS(_m[i * 2 + 1]); //还没做偶数次
	}

}

///////----------------base64

const
char* base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const
uint8_t base64_detable[128] =
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //   0 -   9
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //  10 -  19
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //  20 -  29
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //  30 -  39
	0,  0,  0, 62,  0,  0,  0, 63, 52, 53, //  40 -  49
	54, 55, 56, 57, 58, 59, 60, 61,  0,  0, //  50 -  59
	0, 61,  0,  0,  0,  0,  1,  2,  3,  4, //  60 -  69
	5,  6,  7,  8,  9, 10, 11, 12, 13, 14, //  70 -  79
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, //  80 -  89
	25,  0,  0,  0,  0,  0,  0, 26, 27, 28, //  90 -  99
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38, // 100 - 109
	39, 40, 41, 42, 43, 44, 45, 46, 47, 48, // 110 - 119
	49, 50, 51,  0,  0,  0,  0,  0		// 120 - 127
};


binstream Base64::encode(binstream& mem, bool safe){
	if (mem.size() == 0) return binstream();
	binstream ret;
	int length = (mem.size() / 3 + ((mem.size() % 3 == 0) ? 0 : 1)) * 4;
	ret.resize(length);

	//块处理函数
	auto enblock = [](uint8_t* str, uint8_t* base) {
		*base = base64_table[str[0] >> 2];
		*(base + 1) = base64_table[(str[0] << 4 | str[1] >> 4) & 0x3f];
		*(base + 2) = base64_table[(str[1] << 2 | str[2] >> 6) & 0x3f];
		*(base + 3) = base64_table[str[2] & 0x3f];
	};

	//处理整字节
	for (int i = 0; i < mem.size() / 3; i++) {
		enblock(mem.str() + (i * 3), ret.str() + (i * 4));
	}

	//确定填充部分
	if (mem.size() % 3 != 0) {
		uint8_t* b64_str = ret.end() - 4;
		uint8_t* str_str = mem.end() - (mem.size() % 3);
	
		uint8_t temp[3] = { 0 };
		switch (mem.size() % 3) {
		case 1:
			temp[0] = str_str[0];
			enblock(temp, b64_str);
			*(ret.end() - 2) = '=';
			break;
		case 2:
			temp[0] = str_str[0];
			temp[1] = str_str[1];
			enblock(temp, b64_str);
			break;
		}
		*(ret.end() - 1) = '='; //最后一位一定是 = 
	}

	return ret;
}

binstream Base64::decode(binstream& base, bool safe){
	const uint8_t* det = base64_detable;
	if (base.size() == 0) return binstream();
	binstream ret;
	int length = base.size() / 4 * 3;
	ret.resize(length);

	
	//块处理函数
	auto deblock = [det](uint8_t* str, uint8_t* base) {
		*str = (det[base[0]] << 2) | (det[base[1]] & 0x30) >> 4;
		*(str + 1) = (det[base[1]] << 4) | (det[base[2]]  >> 2);
		*(str + 2) = (det[base[2]] << 6) | det[base[3]];
	};

	//处理处最后一块以外的其他快
	for (int i = 0; i < (base.size() / 4 - 1); i++) {
		deblock(ret.str()+(i*3), base.str()+(i*4));
	}

	//处理最后一块
	if (*(base.end() - 1) == '=') { //是填充块
		uint8_t* baseend = base.end();
		if (*(baseend - 2) == '=') {
			//填充了两个8bit字符
			ret.resize(ret.size() - 2);
			*(ret.end() - 1) = det[baseend[-4]]<<2 | det[baseend[-3]]>>6;
		}else {
			//填充了一个8bit字符
			ret.resize(ret.size() - 1);
			*(ret.end() - 2) = det[baseend[-4]] << 2 | det[baseend[-3]] >> 6;
			*(ret.end() - 1) = det[baseend[-3]] << 4 | det[baseend[-2]] >> 2;
		}
	}else {
		//不是填充
		deblock(ret.end()-3, base.end()-4);
	}
	return ret;
}


_NAP_END

