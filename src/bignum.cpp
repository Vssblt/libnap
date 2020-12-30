#pragma once
#include "nap_common.h"
#include "binstream.h"
#include "bignum.h"
_NAP_BEGIN



BigNum::BigNum(uint64_t n){
	this->set<uint64_t>((uint64_t)n);
}

BigNum::BigNum(int64_t n){
	this->set<int64_t>((int64_t)n);
}

bool BigNum::operator==(const BigNum& other){
	if (this->_sign != other._sign) return false;
	if (this->_length != other._length) return false;
	for (int i = 0; i < this->_length; i++) {
		if (this->_num[i] != other._num[i]) return false;
	}
	return true;
}

bool BigNum::operator!=(const BigNum& other){
	return !((*this) == other);
}

bool BigNum::operator>(const BigNum& other){

	//different sign
	if (!this->_sign && this->_sign) return false;
	if (this->_sign && !this->_sign) return true;

	//equal sign , different length
	if (this->_sign) {
		if (this->_length > other._length) 
			return this->_sign;
		if (this->_length < other._length) 
			return !this->_sign;
	}else {
		if (this->_length > other._length) 
			return this->_sign;
		if (this->_length < other._length) 
			return !this->_sign;
	}

	//equal sign , equal length
	int high_index = this->_length - 1;
	for (int i = high_index; i >= 0; i--) {
		if (this->_num[i] == other._num[i])
			continue;
		
		if (this->_num[i] > other._num[i]) {
			return this->_sign;
		}else{
			return !this->_sign;
		}
	}
	return false; // equal number
}

bool BigNum::operator<(const BigNum& other){
	return !((*this) > other);
}

bool BigNum::operator<=(const BigNum& other){
	if ((*this) < other) return true;
	return ((*this) == other);
}

bool BigNum::operator>=(const BigNum& other){
	// 比较大小
	if ((*this) > other) return true;
	return ((*this) == other);
}

void BigNum::print(std::ostream& out){
	if (this->_length == 0) {
		out << "";
		return;
	}

	if (!this->_sign) {
		out << "-";
	}

	for (int32_t i = this->_length - 1;i >= 0;i--) {
		uint32_t temp = this->_num[i];
		out << std::hex << temp;
	}
}

void BigNum::assign(size_t byte){
	_length = 0;
	_capacity = 0;
	_sign = true;

	if (_num != nullptr) {
		delete[] _num;
		_num = nullptr;
	}

	if (byte == 0) {
		return;
	}

	_num = new uint8_t[byte];
	_capacity = byte;
	memset(_num, 0, byte);
}



_NAP_END