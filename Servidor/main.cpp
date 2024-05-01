#include "client.h"
#include <iostream>
#include <cstring>

int main() {
    // Server details
    std::string server_address = "192.168.1.168";
    int server_port = 5000;

    // Client details
    std::string client_address = "192.168.1.128";
    int client_port = 8888;


    // Create UDP client
    udp_client client(server_address, server_port);
    std::cout << "UDP client is running on " << client.get_addr() << ":" << client.get_port() << std::endl;

    // Message to send
    const char* message = "Hello from client!";
    size_t message_size = std::strlen(message);

    // Send message from client to server
    int bytes_sent = client.send(message, message_size);
    if (bytes_sent < 0) {
        std::cerr << "Error sending message from client to server" << std::endl;
        return 1;
    }

    return 0;
}

