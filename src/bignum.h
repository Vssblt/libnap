#pragma once
#include "nap_common.h"
#include "binstream.h"
_NAP_BEGIN

//not support float
class BigNum {
public:

	template<typename T>
	void set(T n);

	BigNum(uint64_t);
	BigNum(int64_t);

	bool operator==(const BigNum&);
	bool operator!=(const BigNum&);
	bool operator>(const BigNum&);
	bool operator<(const BigNum&);
	bool operator<=(const BigNum&);
	bool operator>=(const BigNum&);

	void print(std::ostream& = std::cout);

protected:

	void assign(size_t byte);


private:

	/*
		High      Low
		  x  ...   0
	*/

	byte* _num = nullptr;
	bool _sign = false; // true +     false -
	uint32_t _length; // byte
	uint32_t _capacity;

	
};



template<typename T>
inline void BigNum::set(T n){
	uint64_t value;

	const size_t size = 8;
	assign(size);

	if (n < 0) {
		this->_sign = false;
		value = -1 * n;
	} else {
		this->_sign = true;
		value = n;
	}

	uint8_t mask = 0xff;

	for (int i = 0; i < size; i++) {
		if (value == 0) {
			break;
		}
		uint8_t temp = value & mask;
		_num[i] = temp;
		value >>= 8;
		this->_length++;
	}
}

//template<> void BigNum::set(uint8_t);
//template<> void BigNum::set(uint16_t);
//template<> void BigNum::set(uint32_t);
//template<> void BigNum::set(uint64_t);
//template<> void BigNum::set(int8_t);
//template<> void BigNum::set(int16_t);
//template<> void BigNum::set(int32_t);
//template<> void BigNum::set(int64_t);

_NAP_END