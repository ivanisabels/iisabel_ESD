#include "Acelerometro.h"
#include "I2C.h"


#define	 MPU_ADDR 0x68
#define	 SMPRT_DIV 0x19
#define	 CONFIG 0x1A
#define	 ACCEL_CONFIG 0x1C
#define	 INT_ENABLE 0x38	// Enables interrupt generation by interrupt sources
#define	 ACCEL_XOUT_H_REG 0x3B	// [15:8]
#define	 ACCEL_XOUT_L_REG 0x3C	// [7:0]
#define	 ACCEL_YOUT_H_REG 0x3D
#define	 ACCEL_YOUT_L_REG 0x3E
#define	 ACCEL_ZOUT_H_REG 0x3F
#define	 ACCEL_ZOUT_L_REG 0x40
#define  PWR_MGMT_1 0x6B
#define  PWR_MGMT_2 0x6C

#define  SEND_MSG_SIZE 1	// Numero mensajes a enviar a MPU
#define  READ_MSG_SIZE 2	// Número mensajes a leer de MPU
#define  W_LEN 2
#define  R_LEN 2

pthread_t hAcelerometro; //Hilo donde se obtendrán los datos del acelerómetro
static uint8_t finalI2C = 1; //Variable para controlar el bucle del hilo


static int MPU_init(int fdI2C);
static int i2c_write(int fdI2C, uint8_t reg_addr, uint8_t value);
static int i2c_read(int fdI2C, uint8_t reg_addr, uint8_t* value);
static int i2c_readRegistros(int fdI2C, uint8_t* reg_addr, uint8_t* value);
static void eliminarHilo(int sig);
static void controlarRupturaPrograma(void);

static void controlarRupturaPrograma(void){
	// Estructura para especificar el comportamiento de sigaction
    struct sigaction sa;

    // Asignar la función de manejador de señales 
    sa.sa_handler = eliminarHilo;
    
    // Limpiar el conjunto de señales a las que se aplica sa_mask
    sigemptyset(&sa.sa_mask);
    
    // Restaurar el comportamiento predeterminado de las señales
    sa.sa_flags = 0;
    
    //Controlamos el programa para cuando se envian señales para cerrar el programa
    sigaction(SIGTERM, &sa, NULL);  //Para cuando se le  envía al programa una señal de terminacion

}

__attribute__((noreturn)) void* acelerometro ( void *arg ) {	
	
	controlarRupturaPrograma();
	
	char i2cFile[15]; //fichero del I2C
	
	int device = 1; //interfaz del periférico I2C
	
	int16_t accel_x = 0; //Aceleración del eje X
	int16_t accel_y = 0; //Aceleración del eje Y
	int16_t accel_z = 0; //Aceleración del eje Z
	uint8_t lecturaRegistrosAcelerometro[6] = {0,0,0,0,0,0};
	uint8_t registrosAcelerometro[] = {ACCEL_XOUT_H_REG, ACCEL_XOUT_L_REG, ACCEL_YOUT_H_REG, ACCEL_YOUT_L_REG, ACCEL_ZOUT_H_REG, ACCEL_ZOUT_L_REG};
	
	float sensibility = 2.0/32767.5; //sensibilidad del acelerómetro
	
	
	
	//Inicializamos el MPU 6050
	if (!MPU_init(fdI2C)) {
		printf("Error en la inicializacion del MPU 6050\n");
		eliminarHilo(0);
	}
	printf("Inicializacion del MPU 6050 correcta");
	
	//Leemos la interrupcion del acelerometro
	uint8_t data_rdy_int = 0;
	if (i2c_read(fdI2C, 0x3A, &data_rdy_int)){
		printf("Interrupcion: %d\n", data_rdy_int);
	}
	
			
	while ( finalI2C ){
		//Dormimos el hilo
		usleep(500000);	
		
		//Limpiamos el terminal
		system("clear");
		
		//Leemos los registros del acelerometro
		//i2c_readRegistros(fdI2C, registrosAcelerometro, lecturaRegistrosAcelerometro);
		for(int n=0; n<6; n++){
			i2c_read(fdI2C, registrosAcelerometro[n], &lecturaRegistrosAcelerometro[n]);
		}
		
		//Obtenemos la aceleración del eje X
		accel_x = (lecturaRegistrosAcelerometro[0] << 8) | (lecturaRegistrosAcelerometro[1]);
		printf("\nAceleracion Eje X: %.5f g", (accel_x*sensibility) );
		
		//Obtenemos la aceleración del eje Y
		accel_y = (lecturaRegistrosAcelerometro[2] << 8) | (lecturaRegistrosAcelerometro[3]);
		printf("\nAceleracion Eje Y: %.5f g", (accel_y*sensibility) );
		
		//Obtenemos la aceleración del eje Z
		accel_z = (lecturaRegistrosAcelerometro[4] << 8) | (lecturaRegistrosAcelerometro[5]);
		printf("\nAceleracion Eje Z: %.5f g", (accel_z*sensibility) );
			
	}
	
	//Cerramos el fichero
	eliminarHilo(0);
	
	//Eliminamos el hilo
	pthread_exit(NULL);

}

static int MPU_init(int fdI2C) {
	
	printf("CONFIGURACION DEL I2C INICIADA\n");
	
	// Configuracion Power management 1
	int device_reset = 0; // [7] Resetea todos los registros internos a su valor por defecto (no funciona si está activado)
	int sleep = 0;	// [6] Modo de bajo consumo
	int cycle = 1;	// [5] Cycle between sleep and waking up to take a single sample of data from accel at LP_WAKE_CTRL rate (sleep must be 0)
	int temp_dis = 1;	// [3] Desactiva el sensor de temperatura Disables temp sensor
	int clksel = 0; // [2:0] 0 = internal 8MHz oscillator Activamos el oscilador interno de 8 MHz
	uint8_t pwr_mgmt_1 = (device_reset << 7) | (sleep << 6) | (cycle << 5) | (temp_dis << 3) | (clksel);
	
		//Enviamos la configuracion al I2C
	if (!i2c_write(fdI2C, PWR_MGMT_1, pwr_mgmt_1)) {
		printf("Error\n");
		close(fdI2C);
		return 0;
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
	if (!i2c_write(fdI2C, PWR_MGMT_2, pwr_mgmt_2)) {
		printf("Error\n");
		eliminarHilo(0);
		return 0;
	}
	printf("Configuracion Power Managemente 2 Exitosa\n");
	
	
	//Especificamos la tasa de muestreo
	uint8_t smplrt_div = 7;	
		//Enviamos el mensaje
	if (!i2c_write(fdI2C, SMPRT_DIV, smplrt_div)) {
		printf("Error\n");
		eliminarHilo(0);
		return 0;
	}
	printf("Tasa de muestreo establecida correctamente\n");
	
	
	//Establecemos el ancho de banda del filtro digital paso bajo
	uint8_t dlpf_cfg = 3;	
		//Enviamos el mensaje
	if (!i2c_write(fdI2C, CONFIG, dlpf_cfg)) {
		printf("Error\n");
		eliminarHilo(0);
		return 0;
	}
	printf("Establecimiento del ancho de banda del filtro exitosa\n");
	
	
	//Selecciona el rango de escala completo de las salidas del acelerometro
	uint8_t afs_sel = 0;	
		//Enviamos el mensaje
	if (!i2c_write(fdI2C, ACCEL_CONFIG, afs_sel)) {
		printf("Error\n");
		eliminarHilo(0);
		return 0;
	}
	printf("Configuracion del rango de escala completada\n");
	
	
	//Activa la interrupcion del valor leido, la cual ocurre en cada momento que se completa una operacion de escritura (se han escrito los valores que solicitamos leer de un registro)
	uint8_t data_rdy_en = 1;	
		//Enviamos el mensaje 
	if (!i2c_write(fdI2C, INT_ENABLE, data_rdy_en)) {
		printf("Error\n");
		eliminarHilo(0);
		return 0;
	}
	printf("INT_ENABLE OK\n");
	
	printf("CONFIGURACION DEL I2C FINALIZADA\n");
	
	return 1;
}

static int i2c_write(int fdI2C, uint8_t reg_addr, uint8_t value) {
	struct i2c_rdwr_ioctl_data packets;
	struct i2c_msg messages[SEND_MSG_SIZE];
	uint8_t write_bytes[W_LEN];
	
	
	//El primer byte indica el registro que queremos escribir
	write_bytes[0] = reg_addr;
	//El segundo byte indica el valor a escribir
	write_bytes[1] = value;
	
	//Creamos el mensaje a enviar
	messages[0].addr	= MPU_ADDR; //Establecemos la dirección del esclavo I2C
	messages[0].flags	= 0;	// Establecemos la operacion de escritura
	messages[0].len		= W_LEN;	// Establecemos los bytes que queremos enviar
	messages[0].buf		= write_bytes; //Establecemos los registros de los que queremos leer
	
	//Creamos el paquete a enviar
	packets.msgs 	= messages;
	packets.nmsgs = SEND_MSG_SIZE;
	
	
	//Enviamos el paquete
	if (ioctl(fdI2C, I2C_RDWR, &packets) < 0) {
		eliminarHilo(0);
		return 0;	// Error
	}
	
	
	return 1;
}

static int i2c_read(int fdI2C, uint8_t reg_addr, uint8_t* value) {
	
	struct i2c_rdwr_ioctl_data packets; //
	struct i2c_msg messages[READ_MSG_SIZE];
	
	// Creamos el mensaje a enviar
	messages[0].addr	= MPU_ADDR; //Establecemos la dirección del esclavo I2C
	messages[0].flags	= 0;	// Establecemos la operacion de escritura
	messages[0].len		= sizeof(*value);	// Establecemos los bytes que queremos enviar
	messages[0].buf		= &reg_addr; //Establecemos los registros de los que queremos leer
	
	// Creamos el mensaje de lectura
	messages[1].addr	= MPU_ADDR; //Establecemos la dirección del esclavo I2C
	messages[1].flags	= I2C_M_RD;	//Establecemos la operacion de lectura
	messages[1].len		= 1; //Establecemos los bytes que queremose enviar
	messages[1].buf		= value; //Esblecemos las variables donde se guardan los registros de los que queremos leer
	
	
	// Creamos el paquete a enviar
	packets.msgs 	= messages; //Mensajes que se enviar
	packets.nmsgs = READ_MSG_SIZE; //Numero de mensajes que contiene el paquete
	
	// Enviamos el paquete
	if (ioctl(fdI2C, I2C_RDWR, &packets) < 0) {
		eliminarHilo(0);
		return 0;	// Error
	}
	
	
	return 1;
}


static int i2c_readRegistros(int fdI2C, uint8_t* reg_addr, uint8_t* value) {
	
	struct i2c_rdwr_ioctl_data packets; //
	struct i2c_msg messages[READ_MSG_SIZE];
	
	// Creamos el mensaje a enviar
	messages[0].addr	= MPU_ADDR; //Establecemos la dirección del esclavo I2C
	messages[0].flags	= 0;	// Establecemos la operacion de escritura
	messages[0].len		= 6;	// Establecemos los bytes que queremos enviar
	messages[0].buf		= reg_addr; //Establecemos los registros de los que queremos leer
	
	// Creamos el mensaje de lectura
	messages[1].addr	= MPU_ADDR; //Establecemos la dirección del esclavo I2C
	messages[1].flags	= I2C_M_RD;	//Establecemos la operacion de lectura
	messages[1].len		= 6; //Establecemos los bytes que queremose enviar
	messages[1].buf		= value; //Esblecemos las variables donde se guardan los registros de los que queremos leer
	
	
	// Creamos el paquete a enviar
	packets.msgs 	= messages; //Mensajes que se enviar
	packets.nmsgs = READ_MSG_SIZE; //Numero de mensajes que contiene el paquete
	
	// Enviamos el paquete
	if (ioctl(fdI2C, I2C_RDWR, &packets) < 0) {
		eliminarHilo(0);
		return 0;	// Error
	}
	
	
	return 1;
}

static void eliminarHilo(int sig){
	//Cerramos el fichero descriptor
	close(fdI2C);
	
	//Dejamos de realizar medidas
	finalI2C = 0;
}
