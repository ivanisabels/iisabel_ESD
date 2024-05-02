#ifndef CLIENT_H
    #define CLIENT_H
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
        #include <cmath> //Pow
        #include "i2c.h"


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
            
            /*-------------- UDP CLIENT ------------------*/
            int                 send(const char *msg, size_t size);
            int                 receive(char *buf, size_t size);
            
            /*-------------- ACCELEROMETER ------------------*/
            void                MPU_init(int fdI2C);
            void                MPU_measure(int16_t measures);

        private:
            int                 f_socket;
            int                 f_port;
            std::string         f_addr;
            struct addrinfo *   f_addrinfo;
        };

#endif
