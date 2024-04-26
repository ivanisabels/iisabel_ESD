#include "client.h"



namespace udp_client_server
{


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
udp_client::udp_client(const std::string& addr, int port)
    : f_port(port)
    , f_addr(addr)
{
    //Specify the sensibility of the accelerometer
    sensibilityAccelerometer = 2.0/32767.5;

     // Create a thread for the client
    std::thread miHilo(&MiClase::clientThread, &objeto);

    //Creation socket file descriptor
    if( f_socket = socket(AF_INET, SOCK_DGRAM, 0) < 0){
        udp_client_server_runtime_error("Creation socket failed");
        close(f_socket);
    }

    //Tenemos que inicializar el Acelerometro
    //Tenemos que inicializar el Colorimetro
    //No se si será mejor crear dos clientes, uno para cada sensor
}

}

/** \brief Clean up the UDP client object.
 *
 * This function frees the address information structure and close the socket
 * before returning.
 */
udp_client::~udp_client()
{
    freeaddrinfo(f_addrinfo);
    close(f_socket);
}

/** \brief Retrieve a copy of the socket identifier.
 *
 * \return The socket used by this UDP client.
 */
int udp_client::get_socket() const
{
    return f_socket;
}

/** \brief Retrieve the port used by this UDP client.
 *
 * \return The port as expected in a host integer.
 */
int udp_client::get_port() const
{
    return f_port;
}

/** \brief Retrieve a copy of the address.
 *
 * \return A string with a copy of the constructor input address.
 */
std::string udp_client::get_addr() const
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
int udp_client::send(const void *msg, size_t size)
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
int udp_client::receive(void *buf, size_t size)
{
    int8_t recv = recvfrom(f_socket, buf, size, 0, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);

    //Check if it has occurred an error
    if( recv == -1){
        udp_client_server_runtime_error("Creation socket failed");
        close(f_socket);
    }
    
    return recv;
}

/** \brief Thread for the client
 */
void clientThread(){
    
    int8_t status = 0; //
    ColorRGB color_Server;
    Color color_Closest;

    //Get the data for the server
        //Especificar el valor de los datos a recibir y la longitud
    while( status == 0){
        status |= receive();

        //Calculate the acceleration of each exis
            //Axis-X
        obtainAccelerationAxis();
            //Axis-Y
        obtainAccelerationAxis();
            //Axis-Z
        obtainAccelerationAxis();

        //Calculate the percentage that a color has of each RGB color
            //Red
        obtainPercentageRGB();
            //Green
        obtainPercentageRGB();
            //Blue
        obtainPercentageRGB();
            //Find the closest color in the color table
        color_Server = { }
        color_Closest = findClosestColor();
    }
}

/** \brief Calculate the acceleration of an axis in the range [-2, 2] g.
 *
 * \param[in] accelerationMSBs  Most Significant Bits of the measure of the acceleration axis
 * \param[in] accelerationLSBs  Less Significant Bits of the measure of the acceleration axis
 *
 * \return Acceleration Axis in the range [-2, 2] g
 */
float obtainAccelerationAxis(uint8_t accelerationMSBs, uint8_t accelerationLSBs){
    int16_t accelerationAxis = (accelerationMSBs << 8) | (accelerationLSBs);

    return accelerationAxis*sensibilityAccelerometer;
}

/** \brief Calculate the percentage that a color has of each RGB color in the range [0, 255].
 *
 * \param[in] accelerationMSBs  Most Significant Bits of the measure of the color
 * \param[in] accelerationLSBs  Less Significant Bits of the measure of the color
 *
 * \return Percentage of the color in the range [0, 255]
 */
float obtainPercentageRGB(uint8_t colorMSBs, uint8_t colorLSBs){
    int16_t color = (colorMSBs << 8) | (colorLSBs);

    return (color*255)/65535;
}

/** \brief Calculate the closest color of the colors' table that we have create
 *
 * \param[in] color Color which we want to know the closest color.
 *
 * \return Closest color of the colors' table
 */
Color findClosestColor(ColorRGB color){
    Color closestColor = colors[0];
    float distanceMinimum;

    float distance(ColorRGB c1, ColorRGB c2) {
        return sqrt( pow(c1.r - c2.r, 2) + pow(c1.g - c2.g, 2) + pow(c1.b - c2.b, 2) );
    }
    

    distanceMinimum = distance(color, colors[0].rgb);

    for (uint8_t i = 1; i < 10; ++i) {
        dist = distance(color, colors[i].rgb);
        if (dist < distanceMinimum) {
            distanceMinimum = dist;
            closestColor = colors[i];
        }
    }

    return closestColor;
}

void RGBC_Init(void){
	
	//Init the TSC3472 
	i2c_write( RGBC_ADDRESS,fdI2C, PWR_CONFIG, POWER_ON);
	
	//Wait 2.4 ms
	usleep(24000);
	
}

void RGBC_meas(void){
	
	uint8_t clear_low, clear_high;
	uint8_t red_low, red_high;
	uint8_t green_low, green_high;
	uint8_t blue_low, blue_high;
	
	//Enable the TSC3472 to do a measure
	i2c_write( RGBC_ADDRESS,fdI2C, PWR_CONFIG , RGBC_MEASURE);
	
	//Wait until measure is done
	usleep(4800);
	
	//Read the measure CLEAR
	i2c_read(RGBC_ADDRESS,fdI2C, CLEAR_L, &clear_low);
	i2c_read(RGBC_ADDRESS,fdI2C, CLEAR_H, &clear_high);

	
	//Read the measure RED
	i2c_read(RGBC_ADDRESS,fdI2C, RED_L, 	&red_low);
	i2c_read(RGBC_ADDRESS,fdI2C, RED_H, 	&red_high);

	
	//Read the measure BLUE
	i2c_read(RGBC_ADDRESS,fdI2C, BLUE_L, &blue_low);
	i2c_read(RGBC_ADDRESS,fdI2C, BLUE_H, &blue_high);

	
	//Read the measure GREEN
	i2c_read(RGBC_ADDRESS,fdI2C, GREEN_L, &green_low); 
	i2c_read(RGBC_ADDRESS,fdI2C, GREEN_H, &green_high);
}

