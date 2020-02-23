#pragma once
#include "nap_common.h"

/*
all function need support 
	char*
	uchar*
	string
	self

functions:
	set
	get
	append
	construct

	+=

	�������� [] ֧�ָ���

*/
_NAP_BEGIN

//	binstream& operator+=(const char*)noexcept;
//	binstream& operator+=(const binstream&)noexcept;
//	binstream& operator+=(const std::string&)noexcept;
//
//	binstream& operator<<(const char*);
//	binstream& operator<<(const binstream&);
//	binstream& operator<<(const std::string&);

////�ַ�������
//	binstream substr(int pos, int length);

////foreach support
//	
//


template<class ST,class SIZET = uint32_t>
class NapStream {
public:

#pragma region binstream transfer

	template <class T>
	static NapStream from(T n) {
		return NapStream(std::to_string(n));
	}

#pragma endregion

#pragma region binstream construct
	static NapStream shift(char** buffer, SIZET len) {
		NapStream<ST,SIZET> naps;
		naps.capacity = len;
		naps.length = len;
		naps.content = (ST*)(*buffer);
		*buffer = nullptr;
		return std::move(naps);
	}

	NapStream() {};

	NapStream(const void* c, SIZET len)noexcept {
		_append((const ST*)(c), len);
	}

	NapStream(const char* c)noexcept {
		_append((const ST*)(c),
			static_cast<const SIZET>(strlen(c)));
	}

	NapStream(const std::string& str)noexcept {
		_append((const ST*)(str.c_str()),
			static_cast<const SIZET>(str.size()));
	}

	NapStream(SIZET len)noexcept {
		_recap(len);
	}

	explicit NapStream(const NapStream& old) noexcept {
		this->clean();
		_append(old.content, old.length);
	}

	NapStream(NapStream&& old) noexcept {
		content = old.content;
		length = old.length;
		capacity = old.capacity;

		old.capacity = 0;
		old.length = 0;
		old.content = nullptr;
	}
#pragma endregion

#pragma region binstream operate


	friend NapStream operator+(const char* s1, NapStream& s2) {
		size_t len = strlen(s1);
		NapStream ret(len + s2.size());
		ret.append(s1,len);
		ret.append(s2);
		return ret;
	}
	friend NapStream operator+(NapStream& s1,const char* s2) {
		size_t len = strlen(s2);
		NapStream ret(len + s1.size());
		ret.append(s1);
		ret.append(s2,len);
		return ret;
	}
	friend NapStream operator+(const std::string& s1, NapStream& s2) {
		NapStream ret(s1.size() + s2.size());
		ret.append(s1);
		ret.append(s2);
		return ret;
	}
	friend NapStream operator+(NapStream& s1, const std::string& s2) {
		NapStream ret(s2.size() + s1.size());
		ret.append(s1);
		ret.append(s2);
		return ret;
	}

	NapStream operator+(const NapStream& s) {
		NapStream ret(size() + s.size());
		ret.append(*this);
		ret.append(s);
		return ret;
	}


#pragma endregion +

#pragma region binstream operate
	bool operator==(const NapStream& o) {
		if (&o == this) return true;
		if (o.length != this->length) return false;
		return (0 == memcmp(o.content, this->content, length));
	}
	bool operator!=(const NapStream& o) {
		return !(*this == o);
	}

	bool operator==(const void* str) {
		SIZET len = static_cast<SIZET>(strlen((const char*)str));
		if (this->length != len) return false;
		return (0 == memcmp(str, this->content, length));
	}
	bool operator!=(const void* str) {
		return !(*this == str);
	}

	bool operator==(const std::string& str) {
		SIZET len = static_cast<SIZET>(str.size());
		if (this->length != len) return false;
		return (0 == memcmp(str.c_str(), this->content, length));
	}
	bool operator!=(const std::string& str) {
		return !(*this == str);
	}
#pragma endregion  == / != 

#pragma region binstream operate
	NapStream& operator=(const NapStream& old) noexcept {
		if (&old != this) {
			this->clean();
			_append(old.content, old.length);
		}
		return *this;
	}

	NapStream& operator=(const std::string& str) noexcept {
		this->clean();
		_append((const ST*)(str.c_str()),
			static_cast<const SIZET>(str.size()));
		return *this;
	}

	NapStream& operator=(const char* str) noexcept {
		this->clean();
		_append((const ST*)(str),
			static_cast<const SIZET>(strlen(str)));
		return *this;
	}
#pragma endregion  =

#pragma region binstream setter

	//���ָ�������ַ�����ɾ����ǰ�ַ�������
	void fill(ST c, SIZET len) {
		_recap(len);
		for (SIZET i = 0; i < len; i++)
			content[i] = c;
		length = len;
	}

	//Ԥ��ָ�����ȿռ䣬��ɾ����ǰ�ַ�������
	//ִ�����lengthΪ0��capacity���ڵ��ڲ���len
	void reserve(SIZET len) {
		_recap(len);
	}

	//���Ŀռ䳤�ȣ�len���С�ڵ�ǰ�ַ������Ȼ�ض�
	//ִ�����lengthΪlen��capacity���ڵ��ڲ���len
	void resize(SIZET len) {
		_recap_hold(len);
	}

	//׷���ַ�
	void append(const void* c, SIZET len) {
		_append(static_cast<const ST*>(c), len);
	}
	void append(const std::string& str) {
		_append(
			(const ST*)(str.c_str()), 
			(const SIZET)str.length()
		);
	}
	void append(const NapStream& str) {
		_append(
			str.content,
			str.size()
		);
	}

	//����������Ϊ0
	inline void clean() noexcept {
		length = 0;
	}


#pragma endregion

#pragma region binstream getter

	inline SIZET size() const { return length; }
	inline SIZET cap() const { return capacity; }

	//��ȡָ��λ���ַ���Խ���׳��쳣
	ST at(SIZET pos) {
		if (pos >= length || pos < 0) {
			throw "Index out of bounds";
			return 0;
		}else {
			return content[pos];
		}
	}

	//��ȡָ��λ���ַ���������Խ����
	ST& operator[](SIZET pos) noexcept{
		return content[pos];
	}

	//��ȡָ��λ���ַ���Խ������
	ST& operator()(int pos) noexcept {
		pos = pos % (int)length;
		if (pos < 0)
			pos = length + pos;
		return content[pos];
	}

	//����һ����׼c++ string
	std::string toStdString() {
		std::string ret(
			(const char* const)(content),
			(const size_t)(length)
		);
		return std::move(ret);
	}

	//��ȡ����0��β���ַ���
	inline ST* str() const{ 
		return content;
	};

	//��ȡ����0��β���ַ���
	inline char* sstr() {
		return (char*)(content);
	};

	//��ȡ��һ���ַ��ĵ�ַ
	uint8_t* begin() { 
		return (uint8_t*)content;
	}

	//��ȡ���һ���ַ�����һ����ַ
	uint8_t* end(){
		return (uint8_t*)content+length;
	}

	//���س����Ƿ�Ϊ0
	inline bool empty() noexcept {
		return (length == 0);
	}
#pragma endregion

#pragma region binstream operate

	friend std::ostream& operator<<(std::ostream& out, const NapStream& b) {
		out.write((const char*)b.content, b.length);
		return out;
	}

	friend std::istream& operator>>(std::istream& is, NapStream& b) {
		while (true) {
			char c = is.get();
			if (!c || c == '\n') break;
			b.append((const char*)c,1);
		}
		return is;
	}

#pragma endregion out<<str  in>>str

	~NapStream() {
		delete[] content;
		content = nullptr;
	}

protected:

	//�ڵ�ǰ�ַ�����׷���ַ���
	//׷�ӳ���Ϊ0���ַ���������ı�ԭ�ַ���
	void _append(const ST* con, SIZET _length) {
		if (_length <= 0 || con == nullptr) return;

		SIZET countlength = _length + length;

		if (countlength > capacity) {
			ST* temp_con = new ST[countlength];

			if (length > 0)//��ֹ֮ǰ�ǿմ�
				memcpy(temp_con, content, static_cast<size_t>(length));

			memcpy(
				temp_con + static_cast<size_t>(length), 
				con,
				static_cast<size_t>(_length)
			);

			delete[] content;
			content = temp_con;
			capacity = countlength;
		}else {
			memcpy(content + length, con, _length);
		}

		length = countlength;
	}

	//����һ���ַ�����������֮ǰ���ַ���
	//���ó���Ϊ0���ַ�����ʹ�ַ�������Ϊ0
	void _set(const ST* con, SIZET _length) {
		assert(_length < 0);
		if (_length < 0) return;
		if (_length == 0) {
			length = 0;
			return;
		}

		if (content == nullptr || _length > capacity) {
			_recap(_length);
		}
		length = _length;
		memcpy(content, con, _length);
	}

	//���·���ռ䣬��СΪcap
	void _recap(SIZET _cap) {
		assert(_cap > 0);//ȷ��Ҫ����Ŀռ����0
		if (_cap <= 0) return;

		if (_cap < static_cast<SIZET>(15))
			_cap = 15;

		if (content != nullptr) {
			delete[] content;
			content = nullptr;
		}

		length = 0;
		capacity = _cap;
		content = new ST[_cap];
	}

	//���·���ռ䣬��СΪcap,�ұ�����ǰ�ռ�����
	void _recap_hold(SIZET _cap) {
		if (_cap < 0) 
			return; //���SIZE_T ���޷����ͣ��˾���Ч
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
			SIZET future_length = _cap;

			if (_cap < static_cast<SIZET>(15))
				_cap = 15;

			if (content != nullptr) {
				ST* temp = content;

				content = new ST[_cap];
				memcpy(content, temp, min<SIZET>(_cap, length));
				delete[] temp;
			}else {
				content = new ST[_cap];
			}
			capacity = _cap;
			length = future_length;
		}	
	}

private:

	ST* content = nullptr;
	SIZET capacity = 0; //content length
	SIZET length = 0;   //string length

};

template class NapStream<uint8_t>;
template class NapStream<char>;
using binstream = NapStream<uint8_t, uint32_t>;

_NAP_END