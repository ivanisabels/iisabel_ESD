#ifndef IOT_CLIENTSEND_H_
#define IOT_CLIENTSEND_H_

#include <string.h>
#include <stdexcept>

class UDPServer_runtime_error: public std::runtime_error {
public:
	UDPServer_runtime_error(const char *msg) : std::runtime_error(msg){

	}
};

class clientReceive {
public:
	clientReceive(const std::string& addr, int port);
    ~clientReceive();

    int GetclientReceiveSocket(void) const;
    int GetclientReceivePort(void) const;
    std::string GetclientReceiveAddr(void) const;

    int Receive(char *msg, size_t maxSize);

private:
    int mclientReceiveSocket;
    int mclientReceivePort;
    std::string mclientReceiveAddr;
    struct addrinfo* mclientReceiveAddrinfo;
};

#endif 
