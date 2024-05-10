//#include "Accelerometer.h"
#include "Colorimeter.h"
#include <iostream>
#include <cstring>


int main() {
    // Server details
    std::string server_address = "192.168.1.168";
    int server_port = 5000;
    // Create UDP client send
    udp_clientSend clientSend(server_address, server_port);
    
    // Client details
    std::string client_address = "192.168.1.228";
    int client_port = 5000;
    // Create UDP client send
    clientReceive clientRec(client_address, client_port);
    
    ////Send Hello Server to the server
    //char message[13] = "Hello Server";
    //clientSend.send( message, sizeof(message));
    
    //std::cout << "Message send, waiting for: Ok" << std::endl;
    
    ////Receive Hello Client for the server
    //char buffer[1024];
    //clientRec.Receive( buffer , sizeof(buffer) ); 
    //std::cout << buffer << std::endl;
    
    
    //Create Accelerometer
    //Accelerometer accele;
    
    //Create Colorimeter
    Colorimeter color;
    
    sleep(3);
    
    //Counter
    uint8_t cnt = 0;
    
    //Measures
    char measures[140];
    
    while(1){
            
        std::cout<<"Prueba entra"<<std::endl;
            
        //Wait 1 second in order to do the measures
        sleep(1);
        
        //Clear the window
        //std::system("clear");
        
        //Reading the accelerometer's measures
        //accele.Accelerometer_measure( &measures[cnt*14] );
        
        usleep(50000);
        
        //Reading the colorimeter's measures
        color.Colorimeter_measure( &measures[cnt*14 + 6] );
        
        //Reading the sensors' measures
        for(uint8_t i = 0; i < 14; i++){
            std::cout << static_cast<uint8_t>(measures[cnt*14 + i]);
        }
        std::cout << std::endl<< std::endl;
        
        //Increment the counter
        cnt++;
        
        // Send message from client to server
        if( cnt == 10 ){
            
            cnt = 0;
            
                        
            // Size of message to send
            //size_t message_size = std::strlen(measures_Char);
            
            //int bytes_sent = client.send(measures_Char, message_size);
            //if (bytes_sent < 0) {
                //std::cerr << "Error sending message from client to server" << std::endl;
                //return 1;
            //}
            
            
        }
    }

    return 0;
}


