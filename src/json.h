#pragma once
#include "binstream.h"
#include "nap_common.h"
#include <exception>
_NAP_BEGIN

#define SKIP_SPACE(N, ARR)                                                     \
  while (ARR[N] == ' ' || ARR[N] == '\r' || ARR[N] == '\n' || ARR[N] == '\t')  \
    N++;

#define L_S '('
#define R_S ')'
#define L_M '['
#define R_M ']'
#define L_L '{'
#define R_L '}'
#define S_Q '\''
#define D_Q '\"'
#define COL ':'
#define COMMA ','

enum BasicJsonType { Number, String, Boolean };

class JsonParser;
class JsonStringify;

/*
 * jsonNode
 * Represents a node of the json array
 */
class JsonNode {
public:
  JsonNode();
  ~JsonNode();

  template <class T> JsonNode(const T &value);

  template <class T> JsonNode(const binstream &key, const T &value);

  // set & copy construct
  template <class T> inline JsonNode &operator=(const T &);

  template <class T> inline JsonNode &operator=(const std::vector<T> &);

  binstream key();
  void key(binstream);

  // Basic type （number,string,float/double,boolean）

  // Getter
  int asInt() const;
  long long asLong() const;
  double asFloat() const;

  std::string asString() const;
  binstream asBinstream() const;

  bool asBool() const;

  // null support
  bool asNull() const;
  void setNull(bool null = true);

  // Object Type
  JsonNode &operator[](const binstream &key);
  void append(binstream key, JsonNode);

  // Array Type
  JsonNode &operator[](size_t index);
  void append(JsonNode);

  size_t size() const;

protected:
  // Basic type assignment
  void set(binstream &str, BasicJsonType);

private:
  binstream _key;

  binstream _bindata;
  BasicJsonType _bintype;

  bool is_null = false;

  std::vector<JsonNode> object_values; // Object array
  std::vector<JsonNode> array_values;  // Normal array

  friend class JsonParser;
  friend class JsonStringify;
};

template <class T> JsonNode::JsonNode(const T &value) { *this = value; }

template <class T> JsonNode::JsonNode(const binstream &key, const T &value) {
  (*this)[key] = value;
}

template <class T>
inline JsonNode &JsonNode::operator=(const std::vector<T> &value) {
  for (int i = 0; i < value.size(); i++) {
    this->array_values.push_back(JsonNode(value[i]));
  }
  return *this;
}

template <class T> inline JsonNode &JsonNode::operator=(const T &value) {
  if (typeid(T) == typeid(float) || typeid(T) == typeid(double) ||
      typeid(T) == typeid(int) || typeid(T) == typeid(long long)) {
    _bintype = Number;
  } else if (typeid(T) == typeid(bool)) {
    _bintype = Boolean;
  } else {
    _bintype = String;
  }
  _bindata = binstream::from<T>(value);
  return *this;
}

template <> inline JsonNode &JsonNode::operator=(const JsonNode &old) {
  // Keep your own this->_key;
  this->_bindata = old._bindata;
  this->is_null = old.is_null;
  this->array_values = old.array_values;
  this->object_values = old.object_values;
  return *this;
}

// Convert json string to class
class JsonParser {
public:
  // JsonParser(bool);
  JsonParser(){};

  bool parse(binstream);

  JsonNode &root();

protected:
  bool parseObject(int &pos, JsonNode &);
  bool parseArray(int &pos, JsonNode &);

  int getStringLen(int pos);
  binstream getString(int pos, int &len);

  int getBooleanLen(int pos);
  int getNumberLen(int pos);
  int getNullLen(int pos);

  bool parseKV(int &pos, JsonNode &);
  bool parseValue(int &pos, JsonNode &);

  void setError(int pos, int msg = 0);

private:
  binstream json;
  JsonNode _root;
};

// Convert class to json string
class JsonStringify {
public:
  static binstream stringify(JsonNode &);

  // Best performance but no escape character transformation.
  static binstream stringifyUnsafe(JsonNode &node);

protected:
  static void strifyKV(JsonNode &, binstream &, bool safe = 1);
  static void strifyValue(JsonNode &, binstream &, bool safe = 1);
  static void dealArray(JsonNode &, binstream &, bool safe = 1);
  static void dealObject(JsonNode &, binstream &, bool safe = 1);
};

_NAP_END
