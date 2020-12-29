#include "binstream.h"
_NAP_BEGIN


void binstream::swap(binstream& b){

	uint8_t* temp_content = this->content;
	size_t temp_capacity = this->capacity;
	size_t temp_length = this->length;

	this->content = b.content;
	this->capacity = b.capacity;
	this->length = b.length;

	b.content = temp_content;
	b.capacity = temp_capacity;
	b.length = temp_length;
}

binstream binstream::shift(char** buffer, size_t len) {
	binstream naps;
	naps.capacity = len;
	naps.length = len;
	naps.content = (uint8_t*)(*buffer);
	*buffer = nullptr;
	return std::move(naps);
}

binstream::binstream(const void* c, size_t len) noexcept {
	_append((const uint8_t*)(c), len);
}

binstream::binstream(const char* c) noexcept {
	_append((const uint8_t*)(c),strlen(c));
}

binstream::binstream(const std::string& str) noexcept {
	_append((const uint8_t*)(str.c_str()),str.size());
}

binstream::binstream(const binstream& old) noexcept {
	this->clean();
	_append(old.content, old.length);
}

//移动构造函数
binstream::binstream(binstream&& old) noexcept {
	content = old.content;
	length = old.length;
	capacity = old.capacity;

	old.capacity = 0;
	old.length = 0;
	old.content = nullptr;
}

binstream binstream::operator+(const binstream& s) {
	binstream ret(size() + s.size());
	ret.append(*this);
	ret.append(s);
	return ret;
}

bool binstream::operator==(const binstream& o) {
	if (&o == this) return true;
	if (o.length != this->length) return false;
	return (0 == memcmp(o.content, this->content, length));
}

bool binstream::operator==(const void* str) {
	size_t len = static_cast<size_t>(strlen((const char*)str));
	if (this->length != len) return false;
	return (0 == memcmp(str, this->content, length));
}

bool binstream::operator==(const std::string& str) {
	size_t len = static_cast<size_t>(str.size());
	if (this->length != len) return false;
	return (0 == memcmp(str.c_str(), this->content, length));
}

binstream& binstream::operator=(const binstream& old) noexcept {
	if (&old != this) {
		_set(old.content, old.length);
	}
	return *this;
}

binstream& binstream::operator=(const std::string& str) noexcept {
	_set((const uint8_t*)(str.c_str()), str.size());
	return *this;
}

binstream& binstream::operator=(const char* str) noexcept {
	_set((const uint8_t*)(str), strlen(str));
	return *this;
}

void binstream::fill(uint8_t c, size_t len) {
	_recap(len);
	for (size_t i = 0; i < len; i++)
		content[i] = c;
	length = len;
}

void binstream::reserve(size_t len) {
	_recap(len);
}

void binstream::resize(size_t len) {
	_recap_hold(len);
}

void binstream::append(const void* c, size_t len) {
	_append(static_cast<const uint8_t*>(c), len);
}

void binstream::append(const std::string& str) {
	_append(
		(const uint8_t*)(str.c_str()),
		str.length()
	);
}

void binstream::append(const binstream& str) {
	_append(
		str.content,
		str.size()
	);
}

uint8_t binstream::at(size_t pos){
	if (pos >= length) {
		throw "Index out of bounds";
		return 0;
	}
	else {
		return content[pos];
	}
}

uint8_t& binstream::operator[](size_t pos) noexcept{
	return content[pos];
}

uint8_t& binstream::operator()(size_t pos) noexcept{
	pos = pos % length;
	return content[pos];
}

std::string binstream::toStdString(){
	std::string ret((const char*)content,length);
	return std::move(ret);
}


binstream::~binstream(){
	if (content)
		delete[] content;
	content = nullptr;
}

void binstream::_append(const uint8_t* con, size_t _length){
	if (_length <= 0 || con == nullptr) return;

	size_t totallength = _length + length;

	if (totallength > capacity) {

		totallength = totallength | 15;

		uint8_t* temp_con = new uint8_t[totallength];

		if (length > 0)//防止之前是空串
			memcpy(temp_con, content, static_cast<size_t>(length));

		memcpy(
			temp_con + static_cast<size_t>(length),
			con,
			static_cast<size_t>(_length)
		);

		delete[] content;
		content = temp_con;
		capacity = totallength;

	}else {
		memcpy(content + length, con, _length);
	}

	length = _length + length;
}
void binstream::_set(const uint8_t* con, size_t _length){
	if (_length == 0) {
		length = 0;
		return;
	}

	if (content == nullptr || _length > capacity) {
		_recap(_length);
	}
	length = _length;
	assert(content != 0);
	memcpy(content, con, _length);
}

void binstream::_recap(size_t _cap){
	if (_cap == 0) {
		capacity = 0;
		length = 0;
		if (content != nullptr) {
			delete[] content;
			content = nullptr;
		}
	}else {

		if (_cap < 15ll)
			_cap = 15ll;

		if (content != nullptr) {
			delete[] content;
			content = nullptr;
		}

		length = 0;
		capacity = _cap;
		content = new uint8_t[_cap];
	}
}

void binstream::_recap_hold(size_t _cap){
	if (_cap == 0) {
		capacity = 0;
		length = 0;
		delete[] content;
		content = nullptr;
		return;
	}

	if (_cap <= capacity) {
		length = _cap;
		return;
	}else {
		size_t future_length = _cap;
		if (_cap < static_cast<size_t>(15))
			_cap = 15;

		if (content != nullptr) {
			uint8_t* temp = content;

			content = new uint8_t[_cap];
			memcpy(content, temp, min<size_t>(_cap, length));
			delete[] temp;
		}else {
			content = new uint8_t[_cap];
		}
		capacity = _cap;
		length = future_length;
	}
}

//friends
binstream operator+(const char* s1, binstream& s2) {
	size_t len = strlen(s1);
	binstream ret(len + s2.size());
	ret.append(s1, len);
	ret.append(s2);
	return ret;
}

binstream operator+(binstream& s1, const char* s2) {
	size_t len = strlen(s2);
	binstream ret(len + s1.size());
	ret.append(s1);
	ret.append(s2, len);
	return ret;
}

binstream operator+(const std::string& s1, binstream& s2) {
	binstream ret(s1.size() + s2.size());
	ret.append(s1);
	ret.append(s2);
	return ret;
}

binstream operator+(binstream& s1, const std::string& s2) {
	binstream ret(s2.size() + s1.size());
	ret.append(s1);
	ret.append(s2);
	return ret;
}

std::ostream& operator<<(std::ostream& out, const binstream& b){
	out.write((const char*)b.content, b.length);
	return out;
}

std::istream& operator>>(std::istream& is, binstream& b){
	while (true) {
		int c[] = { is.get(),0 };
		if (!c[0] || c[0] == EOF || c[0] == '\n') break;
		b.append((const char*)c, 1);
	}
	return is;
}

_NAP_END