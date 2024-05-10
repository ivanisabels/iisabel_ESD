#include "clientReceive.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

clientReceive::clientReceive(const std::string& addr, int port)
    : mclientReceivePort(port), mclientReceiveAddr(addr) {
	
	char decimal_port[16];
    snprintf(decimal_port, sizeof(decimal_port), "%d", mclientReceivePort);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    int r (getaddrinfo(addr.c_str(), decimal_port, &hints, &mclientReceiveAddrinfo));	// Por qué no inicializar con =?

    if (r != 0 || mclientReceiveAddrinfo == NULL) {
    	throw UDPServer_runtime_error(("invalid address or port for UDP socket: \"" + addr + ":" + decimal_port + "\"").c_str());
    }

    mclientReceiveSocket = socket(mclientReceiveAddrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if (mclientReceiveSocket == -1) {
        freeaddrinfo(mclientReceiveAddrinfo);
        throw UDPServer_runtime_error(("could not create UDP socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
    }

    r = bind(mclientReceiveSocket, mclientReceiveAddrinfo->ai_addr, mclientReceiveAddrinfo->ai_addrlen);
    if(r != 0) {
        freeaddrinfo(mclientReceiveAddrinfo);
        close(mclientReceiveSocket);
        throw UDPServer_runtime_error(("could not bind UDP socket with: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
}

clientReceive::~clientReceive() {
    freeaddrinfo(mclientReceiveAddrinfo);
    close(mclientReceiveSocket);
}

int clientReceive::GetclientReceiveSocket(void) const {
    return mclientReceiveSocket;
}

int clientReceive::GetclientReceivePort(void) const {
    return mclientReceivePort;
}

std::string clientReceive::GetclientReceiveAddr(void) const {
    return mclientReceiveAddr;
}

int clientReceive::Receive(char *msg, size_t maxSize) {
    return recv(mclientReceiveSocket, msg, maxSize, 0);		//Estaba como ::recv(...) -> por qué? (scope operator)
}
