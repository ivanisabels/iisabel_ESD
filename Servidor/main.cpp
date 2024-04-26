#include "client.h"
#include "iot_server.h"
#include <iostream>
#include <cstring>

int main() {
    // Server details
    std::string server_address = "127.0.0.1";
    int server_port = 8080;

    // Client details
    std::string client_address = "127.0.0.1";
    int client_port = 8888;

    // Create UDP server
    udp_client_server::udp_server server(server_address, server_port);
    std::cout << "UDP server is running on " << server.get_addr() << ":" << server.get_port() << std::endl;

    // Create UDP client
    udp_client_server::udp_client client(client_address, client_port);
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

    // Receive message from server
    char buffer[1024];
    int bytes_received = server.recv(buffer, sizeof(buffer));
    if (bytes_received < 0) {
        std::cerr << "Error receiving message from server" << std::endl;
        return 1;
    }

    // Print received message
    buffer[bytes_received] = '\0';
    std::cout << "Received message from server: " << buffer << std::endl;

    return 0;
}

