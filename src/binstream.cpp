#include "binstream.h"
_NAP_BEGIN
//
//binstream::binstream():length(0),capacity(0),content(nullptr){}
//
//binstream::binstream(const uint8_t* content, uint32_t len)noexcept {
//	setcontent(content, len);
//}
//binstream::binstream(const std::pair<const char*, uint32_t>& str) noexcept{
//	setcontent(str.first, str.second);
//}
//binstream::binstream(const std::pair<const uint8_t*, uint32_t>& str) noexcept{
//	setcontent(str.first, str.second);
//}
//binstream::binstream(const std::string& s) noexcept{
//	setcontent(s.c_str(), s.length());
//}
//binstream::binstream(const char*c) noexcept{
//	setcontent(c, strlen(c));
//}
//binstream::binstream(const char* content, uint32_t len)noexcept {
//	setcontent(content, len);
//}
//binstream::binstream(uint32_t len)noexcept {
//	renew(len);
//}
//binstream::binstream(binstream& old)noexcept {
//	setcontent(old.content, old.length);
//}
//binstream::binstream(binstream&& old)noexcept {
//	content = old.content;
//	length = old.length;
//	capacity = old.capacity;
//
//	old.capacity = 0;
//	old.length = 0;
//	old.content = nullptr;
//}
//binstream& binstream::operator=(const binstream& other)noexcept {
//	setcontent(other.content,other.length);
//	return *this;
//}
//binstream& binstream::operator=(const std::string& other) noexcept{
//	setcontent(other.c_str(), other.length());
//	return *this;
//}
//binstream& binstream::operator=(const char* other) noexcept{
//	setcontent(other, strlen(other));
//	return *this;
//}
//
//binstream& binstream::operator=(uint8_t* other) noexcept{
//	setcontent(other, strlen((char*)other));
//	return *this;
//}
//
//binstream::~binstream(){
//	delete[] content;
//}
//
//
//
//
//bool binstream::operator==(const binstream& other)
//{
//	if (length != other.length)
//		return false;
//	if (length == 0)
//		return true;
//
//	for (uint32_t i = 0; i < length; i++) {
//		if (content[i] != other.content[i])
//			return false;
//	}
//	return true;
//}
//bool binstream::operator==(const char* s){
//	if (length != strlen(s))
//		return false;
//	if (length == 0)
//		return true;
//
//	for (uint32_t i = 0; i < length; i++) {
//		if (content[i] != s[i])
//			return false;
//	}
//	return true;
//}
//bool binstream::operator==(const int8_t*s){
//	return this->operator==(s);
//}
//bool binstream::operator==(const std::string&s){
//	if (length != s.length())
//		return false;
//	if (length == 0)
//		return true;
//
//	for (uint32_t i = 0; i < length; i++) {
//		if (content[i] != s[i])
//			return false;
//	}
//	return true;
//}
//
//void binstream::append(const binstream& b) noexcept{
//	appendcontent(b.content, b.length);
//}
//
//void binstream::append(const std::string& str) noexcept{
//	appendcontent((const uint8_t*)str.c_str(), str.length());
//}
//
//void binstream::append(const char* b, uint32_t len) noexcept{
//	appendcontent((const uint8_t*)b, len);
//}
//
//void binstream::append(const uint8_t* b, uint32_t len) noexcept{
//	appendcontent(b, len);
//}
//
//void binstream::append(const char* b) noexcept{
//	appendcontent((const uint8_t*)b, strlen(b));
//}
//
//void binstream::append(const char c) noexcept{
//	appendcontent((const uint8_t*)&c, 1);
//}
//
//binstream& binstream::operator+=(const char* cs) noexcept{
//	append(cs);
//	return *this;
//}
//
//binstream& binstream::operator+=(const uint8_t* cs) noexcept{
//	append((char*)cs);
//	return *this;
//}
//
//binstream& binstream::operator+=(const binstream& b) noexcept{
//	append(b);
//	return *this;
//}
//
//binstream& binstream::operator+=(const std::string& str) noexcept{
//	append(str);
//	return *this;
//}
//
//binstream& binstream::operator<<(const char* b) {
//	append(b);
//	return *this;
//}
//
//binstream& binstream::operator<<(const uint8_t*b){
//	append((char*)b);
//	return *this;
//}
//
//binstream& binstream::operator<<(const binstream& b) {
//	append(b);
//	return *this;
//}
//
//binstream& binstream::operator<<(const std::string& b) {
//	append(b);
//	return *this;
//}
//
//
//
//binstream StringDriver::substr(int pos, int len){
//	if ((uint32_t)pos > this->length) return binstream();
//	if ((uint32_t)pos + len > this->length) len = this->length - pos;
//	return binstream(content + pos, len);
//}
//
//std::string binstream::toStdString(){
//	std::string my((char*)content, length);
//	return move(my);
//}
//
//std::pair<const uint8_t*, uint32_t> binstream::toPair(){
//	auto r = std::pair<const uint8_t*, uint32_t>(content, length);
//	return move(r);
//}
//
//uint8_t& binstream::operator[](uint32_t pos) noexcept{
//	return content[pos];
//}
//
//uint8_t binstream::at(uint32_t pos){
//	if (pos >= length) {
//		throw pos;
//		return 0;
//	}else {
//		return content[pos];
//	}
//}
//
//
///////////////////////////////////////////////////
//void binstream::appendcontent(const uint8_t* c, uint32_t _length){
//
//}
//
//
/////////////////////////////////////////////
//
//std::ostream& operator<<(std::ostream& out,const binstream& b){
//	out.write((const char*)b.content, b.length);
//	return out;
//}
//
//std::istream& operator>>(std::istream& is, binstream& b){
//	while (true) {
//		char c = is.get();
//		if (!c || c == '\n') break;
//		b.append(c);
//	}
//
//	return is;
//}



_NAP_END