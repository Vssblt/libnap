#pragma once
#include "nap_common.h"


_NAP_BEGIN

////�ַ�������
//	binstream substr(int pos, int length);

////foreach support
//	
//

class binstream {
public:

#pragma region binstream transfer

	template <class T>
	static binstream from(T n);

#pragma endregion

#pragma region binstream construct
	static binstream shift(char** buffer, size_t len);

	binstream() {};
	binstream(const void* c, size_t len)noexcept;
	binstream(const char* c)noexcept;
	binstream(const std::string& str)noexcept;
	binstream(size_t len)noexcept {_recap(len);}
	explicit binstream(const binstream& old) noexcept;

	binstream(binstream&& old) noexcept;
#pragma endregion

#pragma region binstream operate

	friend binstream operator+(const char* s1, binstream& s2);
	friend binstream operator+(binstream& s1, const char* s2);
	friend binstream operator+(const std::string& s1, binstream& s2);
	friend binstream operator+(binstream& s1, const std::string& s2);

	binstream operator+(const binstream& s);

#pragma endregion +

#pragma region binstream operate

	void operator+=(const binstream& o) {append(o);}
	void operator+=(const char* o) {append(o,strlen(o));}
	void operator+=(const std::string& o) {append(o);}

#pragma endregion +=

#pragma region binstream operate

	bool operator==(const binstream& o);
	inline bool operator!=(const binstream& o) {return !(*this == o);}

	bool operator==(const void* str);
	inline bool operator!=(const void* str) {return !(*this == str);}

	bool operator==(const std::string& str);
	inline bool operator!=(const std::string& str) {return !(*this == str);}

#pragma endregion  == / != 

#pragma region binstream operate

	binstream& operator=(const binstream& old) noexcept;
	binstream& operator=(const std::string& str) noexcept;
	binstream& operator=(const char* str) noexcept;

#pragma endregion  =

#pragma region binstream setter

	//���ָ�������ַ�����ɾ����ǰ�ַ�������
	void fill(uint8_t c, size_t len);

	//Ԥ��ָ�����ȿռ䣬��ɾ����ǰ�ַ�������
	//ִ�����lengthΪ0��capacity���ڵ��ڲ���len
	void reserve(size_t len);

	//���Ŀռ䳤�ȣ�len���С�ڵ�ǰ�ַ������Ȼ�ض�
	//ִ�����lengthΪlen��capacity���ڵ��ڲ���len
	void resize(size_t len);

	//׷���ַ�
	void append(const void* c, size_t len);
	void append(const std::string& str);
	void append(const binstream& str);

	//����������Ϊ0
	void clean() noexcept {length = 0;}

#pragma endregion

#pragma region binstream getter

	inline size_t size() const { return length; }
	inline size_t cap() const { return capacity; }

	//��ȡָ��λ���ַ���Խ���׳��쳣
	uint8_t at(size_t pos);

	//��ȡָ��λ���ַ���������Խ����
	inline uint8_t& operator[](size_t pos) noexcept;

	//��ȡָ��λ���ַ���Խ������
	uint8_t& operator()(size_t pos) noexcept;

	//����һ����׼c++ string
	std::string toStdString();

	//��ȡ����0��β���ַ���
	inline uint8_t* str() const{return content;};

	//��ȡ��һ���ַ��ĵ�ַ
	inline uint8_t* begin() { return (uint8_t*)content;}

	//��ȡ���һ���ַ�����һ����ַ
	inline uint8_t* end(){return (uint8_t*)content+length;}

	//���س����Ƿ�Ϊ0
	inline bool empty() noexcept {return (length == 0);}
#pragma endregion

#pragma region binstream operate

	friend std::ostream& operator<<(std::ostream& out, const binstream& b);
	friend std::istream& operator>>(std::istream& is, binstream& b);

#pragma endregion out<<str  in>>str

	~binstream();
protected:

	//�ڵ�ǰ�ַ�����׷���ַ���
	//׷�ӳ���Ϊ0���ַ���������ı�ԭ�ַ���
	void _append(const uint8_t* con, size_t _length);

	//����һ���ַ�����������֮ǰ���ַ���
	//���ó���Ϊ0���ַ�����ʹ�ַ�������Ϊ0
	void _set(const uint8_t* con, size_t _length);

	//���·���ռ䣬��СΪcap
	void _recap(size_t _cap);

	//���·���ռ䣬��СΪcap,�ұ�����ǰ�ռ�����
	void _recap_hold(size_t _cap);

private:
	uint8_t* content = nullptr;
	size_t capacity = 0; //content length
	size_t length = 0;   //string length
};

template<class T>
inline binstream binstream::from(T n) {
	return binstream(std::to_string(n));
}

_NAP_END