#include "Accelerometer.h"
#include "Colorimeter.h"
#include <iostream>
#include <cstring>


//char* arrayToChar(int16_t matriz[10][7], size_t filas, size_t columnas) {
    //// Calculamos el tamaño total de la cadena, incluyendo el caracter nulo
    //size_t tamanoTotal = filas * columnas + 1;

    //// Creamos un buffer para almacenar la cadena resultante
    //char* buffer = new char[tamanoTotal];
    //char* ptr = buffer;

    //// Copiamos los elementos de la matriz al buffer
    //for (size_t i = 0; i < filas; ++i) {
        //for (size_t j = 0; j < columnas; ++j) {
            //*ptr++ = static_cast<char>(matriz[i][j]);
        //}
    //}

    //// Agregamos el caracter nulo al final
    //*ptr = '\0';

    //return buffer;
//}

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
    
    //Send Hello Server to the server
    clientSend.send("Hello Server", 11);
    
    std::cout << "Message send, waiting for: Ok" << std::endl;
    
    //Receive Hello Client for the server
    char buffer[1024];
    clientRec.Receive( buffer , 2); 
    std::cout << buffer << std::endl;
    
    ////Create Accelerometer
    //Accelerometer accele;
    
    ////Create Colorimeter
    //Colorimeter color;
    
    //sleep(3);
    
    ////Counter
    //uint8_t cnt = 0;
    
    ////Measures
    //int16_t measures[10][7];
    
    //char *measures_Char;
    
    //while(1){
            
        ////Wait 1 second in order to do the measures
        //sleep(1);
        
        ////Clear the window
        //std::system("clear");
        
        ////Reading the accelerometer's measures
        //accele.Accelerometer_measure( &measures[cnt][0] );
        
        //usleep(50000);
        
        ////Reading the colorimeter's measures
        //color.Colorimeter_measure( reinterpret_cast<uint16_t*>(&measures[cnt][3]));
        
        ////Reading the sensors' measures
        //for(uint8_t i = 0; i < 7; i++){
            //std::cout << measures[cnt][i] << " ";
        //}
        //std::cout << std::endl;
        
        ////Increment the counter
        //cnt++;
        
        //// Send message from client to server
        //if( cnt == 10 ){
            
            //cnt = 0;
            
            //measures_Char = arrayToChar(measures, 10, 7);
                        
            //// Size of message to send
            //size_t message_size = std::strlen(measures_Char);
            
            //int bytes_sent = client.send(measures_Char, message_size);
            //if (bytes_sent < 0) {
                //std::cerr << "Error sending message from client to server" << std::endl;
                //return 1;
            //}
            
            
        //}
    //}

    return 0;
}


