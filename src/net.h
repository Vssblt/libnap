#pragma once
#include "nap_common.h"
#include "binstream.h"

_NAP_BEGIN
class net;
class napcom;
class tcpclient;
class tcpserver;
class tcpseraccept;

typedef std::pair<socket_t, sockaddr_in> netobject;
using std::pair;
using std::make_pair;


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

	//��ȡ�ṹ��sockaddr_in�еĶ˿���Ϣ
	//addr Ҫ��ȡ��Ϣ��sockaddr_in
	static unsigned short getPort(sockaddr_in addr);

	//set socket timeout
	static void timeout(socket_t socket, long  millisecond);

	//�����Ƿ�����
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

	//ǿ�Ƶȴ���ȡָ�������ַ���
	static bool recvInsist(
		socket_t sock, 
		char* buf, 
		int size
	);

	//ǿ�Ƶȴ�����ָ�������ַ���
	static bool sendInsist(
		socket_t sock, 
		const char* buf, 
		int size
	);

	//�������ݰ� �ײ㺯��
	static int send(socket_t dest, const char* buf, int size);
	static int send(socket_t dest, const char* buf);
	static int recv(socket_t dest, char* buf, int size);

	net() = delete;
	~net() = delete;
};


//�������ӹ������İ����ͽ�����
/*
	�����ͬʱ���ӹ�socket����
	����napcomͬʱ��ر�socket
*/
class napcom {
public:
	enum class ret {
		success,	//�ɹ�
		empty,	    //����Ϊ��
		mecismsend, //�����ķ������� ������ֻ������ 64KB
		ruined,		//����ٵ������׽���
	};

	napcom(napcom&&) = delete;
	napcom(const napcom&) = delete;
	napcom& operator=(napcom&) = delete;

	ret sendpackage(binstream&);
	ret recvpackage(binstream&);

	~napcom();
private:
	napcom(socket_t&&); //ת������socket�Ŀ���Ȩ

	socket_t net;

	//send
	uint32_t send_seq = 0;
	std::mutex m_send_quene;
	std::queue<binstream> send;

	//recv
	std::mutex m_recv_quene;
	std::queue<binstream> recv;

	bool state = true;
	std::thread* thandler;

	void recvhandle();

	friend class tcpclient;
	friend class tcpseraccept;
};

//����tcp�ķ�����/�ͻ������ӹ�����

class tcpserver {
public:
	tcpserver(uint16_t port, const char* ip = nullptr);
	~tcpserver();
	bool bind();
	bool listen();
	std::pair<bool, tcpseraccept> accept();

	tcpserver(tcpserver&&) = delete;
	tcpserver(const tcpserver&) = delete;
	tcpserver& operator=(tcpserver&) = delete;

	//getter
	inline socket_t getsocket() {return ss;}
	inline sockaddr_in getsockaddr() {return sd;}
	inline netobject netobj() {return netobject(ss, sd);}

private:
	struct sockaddr_in sd;
	socket_t ss;
};

class tcpseraccept {
public:
	
	uint16_t port() { return net::getPort(sd); }
	binstream ip() { return net::getsIp(sd); }
	napcom* communicate() { return easycomm; }

	~tcpseraccept();

	tcpseraccept(tcpseraccept&& old) noexcept;
	tcpseraccept& operator=(tcpseraccept&) = delete;
private:
	tcpseraccept(socket_t, sockaddr_in);
	tcpseraccept() { /*������Ч��accept����*/ };

	napcom* easycomm = nullptr;
	struct sockaddr_in sd = {0,0,0,0};

	friend class tcpserver;
};

class tcpclient {
public:
	tcpclient(uint16_t port, const char* ip);
	~tcpclient();
	tcpclient(tcpclient&&) noexcept;
	tcpclient(const tcpclient&) = delete;
	tcpclient& operator=(tcpclient&) = delete;

	//���ӷ�����
	bool connect();

	//��ȡ�Լ���napcom
	inline napcom* communicate() { return easycomm; }

private:

	void initnapcom();

	netobject net;
	napcom* easycomm  = nullptr;

};

_NAP_END