#include "clientSend.h"


// ========================= CLIENT =========================

/** \brief Initialize a UDP client object.
 *
 * This function initializes the UDP client object using the address and the
 * port as specified.
 *
 * The port is expected to be a host side port number (i.e. 59200).
 *
 * The \p addr parameter is a textual address. It may be an IPv4 or IPv6
 * address and it can represent a host name or an address defined with
 * just numbers. If the address cannot be resolved then an error occurs
 * and constructor throws.
 *
 * \exception udp_client_server_runtime_error
 * The server could not be initialized properly. Either the address cannot be
 * resolved, the port is incompatible or not available, or the socket could
 * not be created.
 *
 * \param[in] addr  The address to convert to a numeric IP.
 * \param[in] port  The port number.
 */
udp_clientSend::udp_clientSend(const std::string& addr, int port)
    : f_port(port)
    , f_addr(addr)
{
    char decimal_port[16];
    snprintf(decimal_port, sizeof(decimal_port), "%d", f_port);
    decimal_port[sizeof(decimal_port) / sizeof(decimal_port[0]) - 1] = '\0';
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    
    int r(getaddrinfo(addr.c_str(), decimal_port, &hints, &f_addrinfo));
    if(r != 0 || f_addrinfo == NULL)
    {
        throw udp_client_server_runtime_error(("invalid address or port: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    
    f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
    if(f_socket == -1)
    {
        freeaddrinfo(f_addrinfo);
        throw udp_client_server_runtime_error(("could not create socket for: \"" + addr + ":" + decimal_port + "\"").c_str());
    }
    
    
}



/** \brief Clean up the UDP client object.
 *
 * This function frees the address information structure and close the socket
 * before returning.
 */
udp_clientSend::~udp_clientSend()
{
    freeaddrinfo(f_addrinfo);
    close(f_socket);
}

int udp_clientSend::CloseClientSendSocket(void) {
	if (0 == close(f_socket)) {
		std::cout << "Server socket closed successfully" << std::endl;
		return 1;
	}
	std::cout << "Error! Couldn't close server's socket" << std::endl;
	return 0;
}

/** \brief Retrieve a copy of the socket identifier.
 *
 * \return The socket used by this UDP client.
 */
int udp_clientSend::get_socket() const
{
    return f_socket;
}

/** \brief Retrieve the port used by this UDP client.
 *
 * \return The port as expected in a host integer.
 */
int udp_clientSend::get_port() const
{
    return f_port;
}

/** \brief Retrieve a copy of the address.
 *
 * \return A string with a copy of the constructor input address.
 */
std::string udp_clientSend::get_addr() const
{
    return f_addr;
}


/** \brief Send a message through this UDP client.
 *
 * \param[in] msg  The message to send.
 * \param[in] size  The number of bytes representing this message.
 *
 * \return -1 if an error occurs, otherwise the number of bytes sent. errno
 * is set accordingly on error.
 */
int udp_clientSend::send(const char *msg, size_t size)
{
    int8_t snd = sendto(f_socket, msg, size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);

    //Check if it has occurred an error
    if( snd == -1){
        udp_client_server_runtime_error("Creation socket failed");
        close(f_socket);
    }

    return snd;
}

/** \brief Receive a message from this UDP client.
 *
 * \param[in] msg  The message to receive.
 * \param[in] size  The number of bytes to receive.
 *
 * \return -1 if an error occurs, otherwise the number of bytes receive. errno
 * is set accordingly on error.
 */
int udp_clientSend::receive(char *buf, size_t size)
{
    int8_t rec = recv(f_socket, buf, size, 0);

    //Check if it has occurred an error
    if( rec == -1){
        udp_client_server_runtime_error("Creation socket failed");
        close(f_socket);
    }
    
    return rec;
}




