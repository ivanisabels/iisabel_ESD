#include "Accelerometer.h"
#include "Colorimeter.h"
#include <iostream>
#include <cstring>
#include "signal.h"
#include <functional>
#include <unistd.h>
// RPI
//#define SERVER_ADDRESS 	"192.168.1.168"
//#define CLIENT_ADDRESS	"192.168.1.108"
//Home
#define SERVER_ADDRESS 	"192.168.1.24"
#define CLIENT_ADDRESS	"192.168.1.148"
#define SERVER_PORT		5000
#define CLIENT_PORT		5000

#define OK 	"OK"
#define NOK "Wrong Message"
#define CLIENT_GREET	"Hello Server"
#define SERVER_GREET	"Hello RPI"

void InitialGreet(void);

//Create Accelerometer
static Accelerometer accele;
//Create Colorimeter
static Colorimeter color;

uint8_t final = 1;

static void closeClient(int sig){

    accele.getI2C().closeFD();
    color.getI2C().closeFD();


    final = 0;
}

static void controlProgramBreak(){
 struct sigaction sa;
 
 sa.sa_handler = closeClient;
 
 sigemptyset(&sa.sa_mask);
 
 sa.sa_flags = 0;
 
 sigaction(SIGINT, &sa, NULL);
 sigaction(SIGTSTP, &sa, NULL);
    
}


int main() {
    
    controlProgramBreak();
    InitialGreet();

    // Server details
    std::string server_address = SERVER_ADDRESS;
    int server_port = SERVER_PORT;
    // Create UDP client send
    udp_clientSend clientSend(server_address, server_port);
    
    // Client details
    std::string client_address = CLIENT_ADDRESS;
    int client_port = CLIENT_PORT;
    // Create UDP client send
    clientReceive clientRec(client_address, client_port);

    sleep(3);
    
    //Counter
    uint8_t contadorMedidas = 0;
    
    //Measures
    char measures[140];
    char buffer[1024];
    
    while(final){
            
        std::cout<<"Measuring:"<<std::endl;
            
        //Wait 1 second in order to do the measures
        //sleep(1);
        usleep(500000);
        
        //Clear the window
        //std::system("clear");
        
        //Reading the accelerometer's measures
        accele.Accelerometer_measure( &measures[contadorMedidas*14] );
        
        usleep(500000);
        
        //Reading the colorimeter's measures
        color.Colorimeter_measure( &measures[contadorMedidas*14 + 6] );
        
        //Reading the sensors' measures
        for(uint8_t i = 0; i < 14; i++){
            std::cout << static_cast<int>(measures[contadorMedidas*14 + i]) ;
        }
        std::cout << std::endl << static_cast<int>(contadorMedidas) << std::endl<< std::endl;
        
        //Increment the counter
        contadorMedidas++;
        
        // Send message from client to server
        if( contadorMedidas == 10 ){
            
            contadorMedidas = 0;
            
                        
            // Size of message to send
            size_t message_size = std::strlen(measures);
            do {
				int bytes_sent = clientSend.send(measures, message_size);
				if (bytes_sent < 0) {
					std::cerr << "Error sending message from client to server" << std::endl;
					break;
				}
				std::cout << "Message sent. Waiting for ack" << std::endl;
				clientRec.Receive(buffer , sizeof(buffer) );
				std::cout << "Message received: " << std::string(buffer) << std::endl;
				usleep(10000);
            } while (std::string(buffer) != OK);
        }
    }




    return 0;
}

void InitialGreet(void) {
	// Server details
	std::string server_address = SERVER_ADDRESS;
	int server_port = SERVER_PORT;
	//Create UDP client send
	udp_clientSend clientSend(server_address, server_port);

	// Client details
	std::string client_address = CLIENT_ADDRESS;
	int client_port = CLIENT_PORT;
	// Create UDP client send
	clientReceive clientRec(client_address, client_port);

	////Send Hello Server to the server
	char message[13] = CLIENT_GREET;
	std::string expectedAns = SERVER_GREET;
	clientSend.send(message, sizeof(message));

	std::cout << "Message sent, waiting for: \"" << expectedAns << "\"" << std::endl;

	//Receive Hello Client from the server
	char buffer[1024];
	clientRec.Receive(buffer , sizeof(buffer) );
	std::cout << "Message received: " << std::string(buffer) << std::endl;

	if (std::string(buffer) == expectedAns) {
		std::cout << "Success! Closing socket" << std::endl;
		clientSend.CloseClientSendSocket();


	}

	usleep(50000);
}


