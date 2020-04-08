#pragma once
#include "nap_common.h"
#include "binstream.h"

_NAP_BEGIN

typedef std::pair<socket_t, sockaddr_in> netobject;

enum socketType {
	udp = SOCK_DGRAM,
	tcp = SOCK_STREAM
};

// recv&send function return value
enum socketRet {
	error = SOCKET_ERROR,
	closed = 0,
	invalid = INVALID_SOCKET,
};

#ifdef WINDOWS
//error function return value
namespace socketError {
	const int timeout = WSAETIMEDOUT;
	const int hostdown = WSAEHOSTDOWN;
};
#endif

#ifdef LINUX
//error function return value
namespace socketError {
	const int timeout = ETIMEDOUT;
	const int hostdown = EHOSTDOWN;
};
#endif

class net {
public:

	//initialization environment
	static void init();

	//fill sockaddr_in struct
	static sockaddr_in make_addr(uint16_t port, const char* ip = nullptr);

	//create legal and nitialization socket handle
	static socket_t socket(socketType type);

	//提取结构体sockaddr_in中的端口信息
	//addr 要获取信息的sockaddr_in
	static unsigned short getPort(sockaddr_in addr);

	//set socket timeout
	static void timeout(socket_t socket, long  millisecond);

	//设置是否阻塞
	static void setclogState(socket_t s,bool block = true);

	////ip transfer
	//warning: Data returned to char* cannot be changed
	static binstream getsIp(sockaddr_in&);  // sockaddr_in => binstream

	static binstream getsIp(uint32_t ip);   // int => binstream

	static uint32_t getnIp(sockaddr_in&);// sockaddr_in => int

	//close socket
	static int32_t close(socket_t socket);

	//get last error
	static int lastError();

	//强制等待获取指定长度字符串
	static bool recvInsist(
		socket_t sock, 
		char* buf, 
		int size
	);

	//强制等待发送指定长度字符串
	static bool sendInsist(
		socket_t sock, 
		const char* buf, 
		int size
	);

	//发送数据包 底层函数
	static int send(socket_t dest, const char* buf, int size);
	static int send(socket_t dest, const char* buf);
	static int recv(socket_t dest, char* buf, int size);

	net() = delete;
	~net() = delete;
};

_NAP_END