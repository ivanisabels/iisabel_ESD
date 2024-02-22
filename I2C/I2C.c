#include <stdio.h>
#include <stdint.h>
#include <sys/ioctl.h> 		// Funcion ioctl
#include <fcntl.h> 		// Funcion open
#include <linux/i2c-dev.h>  	// Para I2C_SLAVE, 
#include <unistd.h>     	// Para close
#include <linux/i2c.h>
#include <signal.h>		// Para las señales que se puedan recibir

typedef struct i2c_rdwr_ioctl_data r;


static void controlarRupturaPrograma(void);
static int configuracionI2C( uint8_t interfaz);
static r paqueteLecturaDatos(uint8_t addr, uint8_t w_len, uint8_t r_len,  char *write_bytes, char *read_bytes);
static void enviarPaquete(struct i2c_rdwr_ioctl_data packets, int fd);
static void leerDatos(char *read_bytes);
static void cerrarPrograma(int fd);


int main(void){

	//Especificamos que debe hacer el programa cuando se le interrumpe mediante una señal de stop o interrupción
	controlarRupturaPrograma();

	//Especificamos los registros de los que queremos leer y donde se guardan sus valores
	char write[1] = { 0x00 };
	char read[1];

	printf("Recopilación de datos de Acelerómetro por medio de I2C\n");

	//Configuramos la dirección del dispositivo I2C y abrimos el fichero I2C para realizar la comunicacion
	int fil_desc = 	configuracionI2C(1);

	//Creamos el paquete con los mensajes de escritura y lectura para leer los registros del acelerómetro
	r paquete = paqueteLecturaDatos( 0x68, sizeof(write), sizeof(read), write, read);

	//Enviamos el paquete al acelerómetro
	enviarPaquete( paquete, fil_desc);

	//Leemos los datos de los registros del acelerómetro
	leerDatos( read );

	//Finalizamos el programa cerando el fichero de configuración I2C
	cerrarPrograma(fil_desc);

}


static void controlarRupturaPrograma(void){
	signal(SIGINT, cerrarPrograma); //Para cuando se le  envía al programa un Control+C
	signal(SIGTSTP, cerrarPrograma);  //Para cuando se le  envía al programa un Control+Z

}

static int configuracionI2C( uint8_t interfaz){

	char i2cFile[15]; //ruta al archivo de comunicación i2c
	int addr = 0x68; //dirección del I2C Slave
	sprintf(i2cFile, "/dev/i2c-%d", interfaz); //copiamos la dirección del archivo de comunicación i2c
	int fd = open(i2cFile, O_RDWR); //obtener descriptor del archivo para lectura
	ioctl(fd, I2C_SLAVE, addr); //configuramos el archivo i2c para la comunicación con el slave

	return fd;
}

static r paqueteLecturaDatos(uint8_t addr, uint8_t w_len, uint8_t r_len,  char *write_bytes, char *read_bytes){
	//Prepare message(s)
	//Structs needed for I2C messages
	struct i2c_rdwr_ioctl_data packets; //paquete que se enviará al i2c, el cual contendrá los mensajes de lectura y escritura
	struct i2c_msg messages[2]; //Establecemos que queremos enviar dos mensajes: uno de escritura y otro de lectura
	//Example of Write message
	messages[0].addr = addr;
	messages[0].flags = 0; //means write
	messages[0].len = w_len;
	messages[0].buf = write_bytes; //Pointer to the data bytes to be written
	// Example of Read message
	messages[1].addr = addr;
	messages[1].flags = 1;  //messages[1].flags = I2C_M_RD
	messages[1].len = r_len;
	messages[1].buf = read_bytes; //Pointer for reading the data

	//Build packet list
	packets.msgs = messages;
	packets.nmsgs = 2;
}

static void enviarPaquete( struct i2c_rdwr_ioctl_data packets, int fd){
	//Send message(s)
	ioctl(fd, I2C_RDWR, &packets);
}

static void leerDatos(char *read_bytes){
	//Parse data received in read_bytes
	printf( "Dato recogido: %c \n", read_bytes[0]);
}

static void cerrarPrograma(int fd){
	//Close file descriptor before exiting program
	close (fd);
}
