#include "json.h"
#include <assert.h>
#include <typeinfo>
_NAP_BEGIN



JsonNode::JsonNode(){
	_bintype = Number;
}

JsonNode::~JsonNode(){}


binstream JsonNode::key(){
	return _key;
}

void JsonNode::key(binstream key){
	_key = std::move(key);
}

int JsonNode::asInt(){
	return _bindata.to<int>();
}

long long JsonNode::asLong(){
	return _bindata.to<long long>();
}

double JsonNode::asFloat(){
	return _bindata.to<double>();
}

binstream JsonNode::asString(){
	return _bindata.toStdString();
}

binstream JsonNode::asBinstream(){
	return _bindata;
}

bool JsonNode::asBool(){
	return _bindata.to<bool>();
}

JsonNode& JsonNode::operator[](const binstream& _key){
	//match key
	for (size_t i = 0; i < object_values.size();i++) {
		if (object_values[i].key() == _key) {
			return object_values[i];
		}
	}
	//create node
	JsonNode newnode;
	newnode.key(_key);
	object_values.push_back(newnode);
	return object_values[object_values.size() - 1];

}

void JsonNode::append(binstream key, JsonNode node){
	node.key(key);
	object_values.push_back(std::move(node));
}

JsonNode& JsonNode::operator[](size_t index){
	if (index >= array_values.size()) {
		auto size = array_values.size();
		JsonNode emptynode;
		for (size_t i = 0ll; i < index - size + 1ull; i++) {
			array_values.push_back(emptynode);
		};
	}
	return array_values[index];
}  

void JsonNode::append(JsonNode node){
	array_values.push_back(std::move(node));
}



void JsonNode::set(binstream& str, BasicJsonType t){
	this->_bindata = str;
	this->_bintype = t;
}

bool JsonNode::asNull(){
	return is_null;
}

void JsonNode::setNull(bool null){
	this->is_null = null;
}

////////////////////


JsonNode& JsonParser::root(){
	return _root;
}

bool JsonParser::parse(binstream str) {
	json = str;
	
	if (json.size() == 0) return true;
	if (json.size() == 1) return false;

	int p = 0;
	try {
		switch (json.at(p)) {
		case L_M:
			return parseArray(p, _root);
			break;
		case L_L:
			return parseObject(p, _root);
			break;
		default:
			setError(p);
			return false;
		}
	}
	catch (BinstreamException& e) {
		this->setError(p, 2);
		return false;
	}
	catch (...) {
		throw JsonException("Parse exception : Unknown Exception");
		return false;
	}
}

bool JsonParser::parseObject(int& pos, JsonNode&node){
	//The character after'{' is a string or'}'
	assert(json.at(pos) == L_L);
	pos++;
	SKIP_SPACE(pos, json);

	switch (json.at(pos)) {
	case R_L:
		//Absorb the ending character because the character belongs to this object
		pos++; 
		return true; //Empty object
	case D_Q: 
		while(1){
			if (!parseKV(pos, node)) return false;
			SKIP_SPACE(pos, json);
			if (json.at(pos) == COMMA) {
				pos++;
				continue;
			}else if(json.at(pos) == R_L){
				pos++; //Remove closing parenthesis
				return true;
			}else {
				setError(pos);
				return false;
			}
		}
		break;
	default:
		setError(pos);
		return false;
	}
	assert(false);
	return false;
}

bool JsonParser::parseArray(int& pos, JsonNode& node){
	//'[' is expected to be string, number, null, boolean, left brace, left square bracket, right square bracket.
	assert(json.at(pos) == L_M);
	pos++;
	SKIP_SPACE(pos, json);

	switch (json.at(pos)) {
	case R_M:
		//Absorb the ending character because the character belongs to this object
		pos++;
		return true; //Empty array

	default:
		while (1) {
			JsonNode _node;
			if (!parseValue(pos, _node)) return false;
			node.append(_node);

			SKIP_SPACE(pos, json);
			if (json.at(pos) == COMMA) {
				pos++;
				continue;
			}else if (json.at(pos) == R_M) {
				pos++; //Remove closing parenthesis
				return true;
			}else {
				setError(pos);
				return false;
			}
		}
	}
	assert(false);
	return false;
}

int JsonParser::getStringLen(int pos) {
	assert(json.at(pos) == D_Q);
	pos++;

	int len = 0;
	while (pos < (int)json.size()) {
		if (json.at(pos) == '\\') {
			len++;
			pos++;
		}else {
			if (json.at(pos) == D_Q) return len;
		}
		
		len++;
		pos++;
	}
	return -1;
}

binstream JsonParser::getString(int pos,int& len){
	binstream value(json.str() + pos + 1, len);
	binstream ret;
	if (len == 0) return "";
	ret.reserve(value.size());
	int p1 = 0, p2 = 0;
	while (p1 < len) {
		char c = value[p1];
		if (c == '\\') {
			p1++;
			continue;
		}
		ret[p2] = value[p1];
		p1++;
		p2++;
	}
	ret.resize(p2);
	return ret;
}

void JsonParser::setError(int pos, int msg){
	binstream errorinfo = "JsonException - ";
	const char* errormsg[] = {
		"Unexpected character at position ", //出现意外的字符
		"Parse exception: Unknown Exception",//未知原因的解析异常
		"Parse exception: ",	//解析异常
		"Unclosed string at pos: ", //未闭合的字符串
		"Illegal character at position " //不合法的字符
	};
	errorinfo += errormsg[msg];
	errorinfo += binstream::from<int>(pos);
	throw JsonException(errorinfo.toStdString().c_str());
	return;
}

int JsonParser::getBooleanLen(int pos) {
	if (json.at(pos) == 't') {
		if (json.at(pos+1ll) == 'r' &&
			json.at(pos+2ll) == 'u' &&
			json.at(pos+3ll) == 'e')
			return 4;
	}
	if (json.at(pos) == 'f') {
		if (json.at(pos+1ll) == 'a' &&
			json.at(pos+2ll) == 'l' &&
			json.at(pos+3ll) == 's' &&
			json.at(pos+4ll) == 'e')
			return 5;
	}
	return -1;
}

//A return value of 0 means an error
int JsonParser::getNumberLen(int pos) {
	int len = 0;
	while (pos < (int)json.size()) {
		char c = json.at(pos);
		if (len == 0 || c == '-') {
			len++;
			pos++;
			continue;
			//Only the first digit of the number can be a minus sign
		}
		if ((c <= '9' && c >= '0') || c == '.') {
			len++;
			pos++;
			continue;
		}
		break;
	}
	return len;
}

int JsonParser::getNullLen(int pos) {
	if (json.at(pos) == 'n') {
		if (json.at(pos + 1) == 'u' &&
			json.at(pos + 2) == 'l' &&
			json.at(pos + 3) == 'l')
			return 4;
	}
	return -1;
}

bool JsonParser::parseKV(int& pos, JsonNode& node){
	//Key-Value requires 
	//  a string at the beginning, a colon in the middle, and a legal type at the end
	SKIP_SPACE(pos, json);
	if (json.at(pos) != D_Q) {
		setError(pos, 3);
		return false;
	}
	
	int len = getStringLen(pos);
	if (len == -1) {
		setError(pos, 3);
		return false;
	}
	binstream key(json.str() + pos + 1, len);
	JsonNode& newnode = node[key];
	pos += len + 2; //point to next character

	SKIP_SPACE(pos, json);
	if (json.at(pos) != COL) {
		setError(pos);
		return false;
	}else {
		pos++; //消化冒号
	}
	return parseValue(pos, newnode);
}

bool JsonParser::parseValue(int& pos, JsonNode& node) {
	//Expected after the colon is string, number, null, boolean, left brace, left square bracket
	//The colon is not part of the value part and should be removed by the caller
	SKIP_SPACE(pos, json);

	//Determine the value type
	switch (json.at(pos)){
	case L_L:
		return parseObject(pos, node);

	case L_M:
		return parseArray(pos, node);

	case 't': {//boolean
		int len = getBooleanLen(pos);
		if (len == 4) {
			node = true;
			pos += len;
			return true;
		}else {
			setError(pos, 4);
			return false;
		}
	}
	case 'f': { //boolean
		int len = getBooleanLen(pos);
		if (len == 5) {
			node = false;
			pos += len;
			return true;
		}
		else {
			setError(pos, 4);
			return false;
		}
	}
	case 'n': { // null
		int len = getNullLen(pos);
		if (len == -1) {
			setError(pos, 4);
			return false;
		}
		else {
			node.setNull(true);
			pos += len;
			return true;
		}
	}
	case D_Q: {//string
		int len = getStringLen(pos);
		if (len == -1) {
			setError(pos, 3);
			return false;
		}
		auto temp = getString(pos, len);
		node = temp;
		pos += len + 2; //point to next character
		return true;
	}
	default:
		//Determine whether it is a number or an illegal type
		if (json.at(pos) == '-' || (json.at(pos) <= '9' && json.at(pos) >= '0')) {
			int len = getNumberLen(pos);
			if (len == 0) {
				setError(pos, 3);
				return false;
			}
			binstream value(json.str() + pos, len);
			node.set(value,Number);
			pos += len;
			return true;
		}else {
			setError(pos, 4);
			return false;
		}
		break;
	}
	assert(false);
	return false;
}

/////////////////////


binstream JsonStringify::stringify(JsonNode& node){
	binstream str;
	if (node.array_values.size() != 0) {
		//Array
		dealArray(node, str);
	}else if (node.object_values.size() !=0 ) {
		//Object
		dealObject(node, str);
	}else {
		str = "{}";
	}
	return str;
}

void JsonStringify::strifyKV(JsonNode& node, binstream& str){
	str += "\"";
	str += node.key();
	str += "\"";
	str += ":";
	strifyValue(node, str);
}


void JsonStringify::strifyValue(JsonNode& node, binstream& str){
	//Determine what type is next
	if (node.asNull()) { //Determine if it is null
		str += "null";
		return;
	}

	if (node.array_values.size() != 0) {
		//Array
		dealArray(node, str);
		return;
	}
		
	if (node.object_values.size() != 0) {
		//Object
		dealObject(node, str);
		return;
	}
	
	//Determine whether it is other basic types
	switch (node._bintype) {
	case Number:
		str += node._bindata;
		break;
	case String: {
		str += "\"";

		for (char c : node._bindata) {
			if (c == '\"') {
				str += "\\\"";
			}else {
				str += c;
			}
		}
		
		str += "\"";
		break; 
	}
	case Boolean:
		if (node._bindata.to<bool>()) {
			str += "true";
		}else {
			str += "false";
		}
		break;
	}
}

void JsonStringify::dealArray(JsonNode& node, binstream& str){
	str += "[";
	for (JsonNode& n : node.array_values) {
		strifyValue(n, str);
		str += ",";
	}
	str[str.size()-1] = ']';
}

void JsonStringify::dealObject(JsonNode& node, binstream& str){
	str += "{";
	for (JsonNode& n : node.object_values) {
		strifyKV(n, str);
		str += ",";
	}
	str[str.size() - 1] = '}';
}

_NAP_END

