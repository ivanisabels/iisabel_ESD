#include "client.h"


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

    //Creation socket file descriptor
    if( f_socket = socket(AF_INET, SOCK_DGRAM, 0) < 0){
        udp_client_server_runtime_error("Creation socket failed");
        close(f_socket);
    }

    //Initialice the accelerometer
    //I2CDevice::init_i2c(1,MPU_ADDR);
    //udp_client::MPU_init( I2CDevice::get_fd() );
    
    //Tenemos que inicializar el Colorimetro
    
    
    //No se si será mejor crear dos clientes, uno para cada sensor
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
int udp_client::send(const char *msg, size_t size)
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
int udp_client::receive(char *buf, size_t size)
{
    int8_t rec = recv(f_socket, buf, size, 0);

    //Check if it has occurred an error
    if( rec == -1){
        udp_client_server_runtime_error("Creation socket failed");
        close(f_socket);
    }
    
    return rec;
}


/*-------------- ACELEROMETRO ------------------*/
/*void udp_client::MPU_init(int fdI2C) {





	
	printf("CONFIGURACION DEL I2C INICIADA\n");
	
	// Configuracion Power management 1
	int device_reset = 0; // [7] Resetea todos los registros internos a su valor por defecto (no funciona si está activado)
	int sleep = 0;	// [6] Modo de bajo consumo
	int cycle = 1;	// [5] Cycle between sleep and waking up to take a single sample of data from accel at LP_WAKE_CTRL rate (sleep must be 0)
	int temp_dis = 1;	// [3] Desactiva el sensor de temperatura Disables temp sensor
	int clksel = 0; // [2:0] 0 = internal 8MHz oscillator Activamos el oscilador interno de 8 MHz
	uint8_t pwr_mgmt_1 = (device_reset << 7) | (sleep << 6) | (cycle << 5) | (temp_dis << 3) | (clksel);
	
	//Enviamos la configuracion al I2C
	if ( i2c_write(MPU_ADDR, fdI2C, PWR_MGMT_1, pwr_mgmt_1) == -1) {
		printf("Error al configurar el I2C: Power Management 1\n");
		eliminarHilo(0);
	}
	printf("Configuracion Power Management 1 Exitosa\n");
	
	
	//Configuracion Power management 2
	uint8_t lp_wake_ctrl = 3 << 6; // [7:6] Wake-up frequency = 40Hz = 25ms
	int stby_xa = 0 << 5;	// [5] Modo de Bajo Consumo Eje X Acelerometro
	int stby_ya = 0 << 4;	// [4] Modo de Bajo Consumo Eje Y Acelerometro
	int stby_za = 0 << 3;	// [3] Modo de Bajo Consumo Eje Z Acelerometro
	int stby_xg = 1 << 2;	// [2] Modo de Bajo Consumo Eje X Giroscopio
	int stby_yg = 1 << 1;	// [1] Modo de Bajo Consumo Eje Y Giroscopio
	int stby_zg = 1;	// [0] Modo de Bajo Consumo Eje Z Giroscopio
	uint8_t pwr_mgmt_2 = (lp_wake_ctrl) | (stby_xa) | (stby_ya) | (stby_za) | (stby_xg) | (stby_yg) | (stby_zg);
	
	//Enviamos la configuracion al I2C
	if ( i2c_write(MPU_ADDR, fdI2C, PWR_MGMT_2, pwr_mgmt_2) == -1) {
		printf("Error al configurar al I2C: Power Management 2\n");
		eliminarHilo(0);
	}
	printf("Configuracion Power Managemente 2 Exitosa\n");
	
	
	//Especificamos la tasa de muestreo
	uint8_t smplrt_div = 7;	
		//Enviamos el mensaje
	if ( i2c_write(MPU_ADDR, fdI2C, SMPRT_DIV, smplrt_div) == -1) {
		printf("Error al especificar la tasa de muestreo\n");
		eliminarHilo(0);
	}
	printf("Tasa de muestreo establecida correctamente\n");
	
	
	//Establecemos el ancho de banda del filtro digital paso bajo
	uint8_t dlpf_cfg = 3;	
		//Enviamos el mensaje
	if ( i2c_write(MPU_ADDR, fdI2C, CONFIG, dlpf_cfg)  == -1) {
		printf("Error al establecer el ancho de banda del filtro digital paso bajo\n");
		eliminarHilo(0);
	}
	printf("Establecimiento del ancho de banda del filtro exitosa\n");
	
	
	//Selecciona el rango de escala completo de las salidas del acelerometro
	uint8_t afs_sel = 0;	
		//Enviamos el mensaje
	if ( i2c_write(MPU_ADDR, fdI2C, ACCEL_CONFIG, afs_sel)  == -1) {
		printf("Error al seleccionar el rango de escala completo de las salidas del acelerometro\n");
		eliminarHilo(0);
	}
	printf("Configuracion del rango de escala completada\n");
	
	
	//Activa la interrupcion del valor leido, la cual ocurre en cada momento que se completa una operacion de escritura (se han escrito los valores que solicitamos leer de un registro)
	uint8_t data_rdy_en = 1;	
		//Enviamos el mensaje 
	if ( i2c_write(MPU_ADDR, fdI2C, INT_ENABLE, data_rdy_en)  == -1) {
		printf("Error al activar la interrupción del valor leído\n");
		eliminarHilo(0);
	}
	printf("INT_ENABLE OK\n");
	
	printf("CONFIGURACION DEL I2C FINALIZADA\n");
	
}
* */


