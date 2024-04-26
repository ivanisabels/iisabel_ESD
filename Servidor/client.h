#ifndef SNAP_UDP_CLIENT_SERVER_H
#define SNAP_UDP_CLIENT_SERVER_H

#include <sys/socket.h>
#include <netdb.h> //Struct addrinfo
#include <stdexcept>
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <thread> //Thread
#include <cmath> //Pow

namespace udp_client_server
{

class udp_client_server_runtime_error : public std::runtime_error
{
public:
    udp_client_server_runtime_error(const char *w) : std::runtime_error(w) {}
};


class udp_client
{
public:
                        udp_client(const std::string& addr, int port);
                        ~udp_client();

    int                 get_socket() const;
    int                 get_port() const;
    std::string         get_addr() const;

    int                 send(const char *msg, size_t size);
    int                 receive(void *buf, size_t size);

    void                clientThread();

private:
    int                 f_socket;
    int                 f_port;
    std::string         f_addr;
    struct addrinfo *   f_addrinfo;
    
    //Accelerometer
    float               sensibilityAccelerometer;

    //Colorimeter
    typedef struct {uint16_t r;uint16_t g;uint16_t b;} ColorRGB;
    typedef struct { char* nombre; ColorRGB rgb;} Color;
    Color colors[] = {
        {"Red", {255, 0, 0}},
        {"Green", {0, 255, 0}},
        {"Blue", {0, 0, 255}},
        {"Yellow", {255, 255, 0}},
        {"Purple", {128, 0, 128}},
        {"Cyan", {0, 255, 255}},
        {"Orange", {255, 165, 0}},
        {"Brown", {139, 69, 19}},
        {"White", {255, 255, 255}},
        {"Black", {0, 0, 0}},
    };

    int8_t              obtainAccelerationAxis(uint8_t accelerationMSBs, uint8_t accelerationLSBs);
    
    float               obtainPercentageRGB(uint8_t colorMSBs, uint8_t colorLSBs);
    float               findClosestColor(ColorRGB color);
};

}
