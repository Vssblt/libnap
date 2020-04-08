#include "net.h"
#pragma warning(disable:4996)
_NAP_BEGIN


void net::init(){
#ifdef WINDOWS 
	//init winsock wsa
	WSAData wsadata;
	int i = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (i != 0) {
		std::cerr << "WSAStartup Fail";
		assert(i == 0);
	}
#endif
}

sockaddr_in net::make_addr(uint16_t port, const char* ip){
	sockaddr_in addr;
	addr.sin_addr.s_addr = (ip == nullptr) ? INADDR_ANY : inet_addr(ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	return addr;
}

socket_t net::socket(socketType type){
	socket_t ret;
	ret = ::socket(AF_INET, type, 0);
	return ret;
}

unsigned short net::getPort(sockaddr_in addr){
	return ntohs(addr.sin_port);
}

void net::timeout(socket_t socket, long millisecond){
#ifdef LINUX
	if (millisecond < 1000)
		millisecond = 1000;
	struct timeval tv_out;
	tv_out.tv_sec = millisecond/1000;
	tv_out.tv_usec = 0;
	setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv_out,sizeof(tv_out));
#endif 

#ifdef WINDOWS
	int timeout = millisecond;
	setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

#endif

}

void net::setclogState(socket_t s, bool block){
#ifdef LINUX
	int flags = fcntl(s, F_GETFL, 0);        //获取文件的flags值。
	if (block)
		fcntl(s, F_SETFL, flags &~ O_NONBLOCK);//block
	else
		fcntl(s, F_SETFL, flags | O_NONBLOCK);//not block
#endif

#ifdef WINDOWS
	u_long u;
	if (block)
		u = 0; // block
	else
		u = 1; //not block
	::ioctlsocket(s, FIONBIO, &u);
#endif
}

binstream net::getsIp(sockaddr_in& sa){
	//return inet_ntoa(sa.sin_addr);
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET,&sa.sin_addr, ip, sizeof(ip));
	return std::move(binstream(ip));
}

binstream net::getsIp(uint32_t ip){
	ip = ntohl(ip);
	uint8_t* p = (uint8_t*)&ip;
	char* buffer = new char[16];
	memset(buffer, 0, 16);
	sprintf(buffer, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
	return binstream::shift(&buffer, strlen(buffer));
}

uint32_t net::getnIp(sockaddr_in& object){
	return ntohl(object.sin_addr.s_addr);
}

int32_t net::close(socket_t socket){

#ifdef LINUX
	int32_t r =  ::close(socket);
#endif

#ifdef WINDOWS
	int32_t r = ::closesocket(socket);
#endif

	socket = invalid;
	return r;
}

int net::lastError()
{
	int err = 0;
#ifdef LINUX
	err = errno;
#endif

#ifdef WINDOWS
	err =  ::WSAGetLastError();
#endif
	
	return err;
}

bool net::recvInsist(socket_t sock,char* buf, int size){
	assert(size > 0);
	int count = 0;
	while (count < size) {
		int n = ::recv(sock,buf + count, size - count, MSG_NOSIGNAL);
		if (n == socketRet::closed || n == socketRet::error || n<=0) {
			return false;
		}
		count += n;
	}
	return true;
}

bool net::sendInsist(socket_t sock,const char* buf, int size){
	assert(size > 0);
	int count = 0;
	while (count < size) {
		int n = ::send(sock, buf+count, size - count, MSG_NOSIGNAL);
		if (n == socketRet::closed || n == socketRet::error || n <= 0) {
			return false;
		}
		count += n;
	}
	return true;
}


int net::send(socket_t dest, const char* buf, int size) {
	return ::send(dest, buf, size, MSG_NOSIGNAL);
}

int net::send(socket_t dest, const char* buf) {
	return net::send(dest, buf, static_cast<int>(strlen(buf)));
}

int net::recv(socket_t dest, char* buf, int size) {
	return ::recv(dest, buf, size, MSG_NOSIGNAL);
}

_NAP_END