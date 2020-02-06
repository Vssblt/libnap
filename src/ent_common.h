#pragma once
#include <string>
#include <time.h>
#include <random>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <array>
#include <limits.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>


typedef unsigned char byte;

#define _NAP_BEGIN namespace nap{
#define _NAP_END }
//#define _NAP ::nap::

//linux sipport
#ifdef __linux__
	#define PLATFORM LINUX
	#define LINUX
	#define SOCKET int
	#define SOCKET_ERROR (-1)  
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <netdb.h>
	#include <netinet/ip.h>
	#include <netinet/in.h>
	#include <string.h>	
	#include <unistd.h>
	#include <arpa/inet.h>
	#include <fcntl.h> 
	#include <errno.h> 
	
	#define ULONG long
	#define INVALID_SOCKET  (SOCKET)(~0)
	typedef int socket_t;
#endif

//windows support
#ifdef _WIN32
	#define WINDOWS
	#ifdef _WIN64
		#define PLATFORM WIN64
	#else
		#define PLATFORM WIN32
	#endif
	#define NOMINMAX //Disable vc++ min/max
	#include <WinSock2.h>
	#include <ws2tcpip.h>
	#pragma comment (lib,"ws2_32")
	#define MSG_NOSIGNAL 0
	/*#define MSG_DONTWAIT 0*/

	typedef SOCKET socket_t;
#endif

#ifndef PLATFORM
	#pragma message("Unsupported Platform")
	#pragma hdrstop
#endif	


_NAP_BEGIN

//public tool function

//Simple hash function
uint32_t mhash(const char* str, uint32_t len);

//Get the current timestamp
uint32_t timestamp();

_NAP_END