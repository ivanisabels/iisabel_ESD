#include "I2C.h"

int fdI2C; //Fichero descriptor para leer

void init_i2c( int device, int addr){
	
	char i2cFile[15];
	
	//Guardamos 
	sprintf(i2cFile, "/dev/i2c-%d", device);
	//Obtenemos el fichero descriptor para escritura
	fdI2C = open(i2cFile, O_RDWR);
	if (!fdI2C) { //No se puede abrir el fichero
		printf("No se puede abrir el fichero %s\n", FILE_NAME);
	}
	printf("El fichero: %s, se ha abierto correctamente\n", i2cFile);
	
	//Configuracion del fichero para las comunicaciones I2C con el esclavo
	if (ioctl(fdI2C, I2C_SLAVE, addr) < 0) {
		printf("ioctl error\n");
	}
	printf("Configuracion I2C satisfactoria\n");
}

int i2c_write(uint8_t addr, int fdI2C, uint8_t reg_addr, uint8_t value) {
	struct i2c_rdwr_ioctl_data packets;
	struct i2c_msg messages[SEND_MSG_SIZE];
	uint8_t write_bytes[W_LEN];
	
	
	//El primer byte indica el registro que queremos escribir
	write_bytes[0] = reg_addr;
	//El segundo byte indica el valor a escribir
	write_bytes[1] = value;
	
	//Creamos el mensaje a enviar
	messages[0].addr	= addr; //Establecemos la dirección del esclavo I2C
	messages[0].flags	= 0;	// Establecemos la operacion de escritura
	messages[0].len		= W_LEN;	// Establecemos los bytes que queremos enviar
	messages[0].buf		= write_bytes; //Establecemos los registros de los que queremos leer
	
	//Creamos el paquete a enviar
	packets.msgs 	= messages;
	packets.nmsgs = SEND_MSG_SIZE;
	
	
	//Enviamos el paquete
	if (ioctl(fdI2C, I2C_RDWR, &packets) < 0) {
		return 0;	// Error
	}
	
	
	return 1;
}

int i2c_read(uint8_t addr, int fdI2C, uint8_t reg_addr, uint8_t* value) {
	
	struct i2c_rdwr_ioctl_data packets; //
	struct i2c_msg messages[READ_MSG_SIZE];
	
	// Creamos el mensaje a enviar
	messages[0].addr	= addr; //Establecemos la dirección del esclavo I2C
	messages[0].flags	= 0;	// Establecemos la operacion de escritura
	messages[0].len		= sizeof(*value);	// Establecemos los bytes que queremos enviar
	messages[0].buf		= &reg_addr; //Establecemos los registros de los que queremos leer
	
	// Creamos el mensaje de lectura
	messages[1].addr	= addr; //Establecemos la dirección del esclavo I2C
	messages[1].flags	= I2C_M_RD;	//Establecemos la operacion de lectura
	messages[1].len		= sizeof(*value); //Establecemos los bytes que queremose enviar
	messages[1].buf		= value; //Esblecemos las variables donde se guardan los registros de los que queremos leer
	
	
	// Creamos el paquete a enviar
	packets.msgs 	= messages; //Mensajes que se enviar
	packets.nmsgs = READ_MSG_SIZE; //Numero de mensajes que contiene el paquete
	
	// Enviamos el paquete
	if (ioctl(fdI2C, I2C_RDWR, &packets) < 0) {
		return 0;	// Error
	}
	
	
	return 1;
}
