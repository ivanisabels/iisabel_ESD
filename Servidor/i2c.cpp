#include "i2c.h"
#include <iostream>	//
#include <string>
I2CDevice::I2CDevice(uint8_t devNum, uint8_t devAddr) {
			this->devNum = devNum;
			this->devAddr = devAddr;
		}
		
int I2CDevice::init(uint8_t device, uint8_t dev_addr) 
{
	//String with the file descriptor path
	std::string i2cFile= "/dev/i2c-" + std::to_string(device);
	
	//Open the file descriptor
	
	fd = open(i2cFile.c_str(), O_RDWR);
	
	if (fd < 0) {
		std::cout << "Error: can't open the file " << i2cFile << std::endl;
		return -1;
	}
	
	std::cout << "File " << i2cFile << " opened succesfully" << std::endl;
	
	// Configure the file fot I2C communications with slave at the specified address
	if (ioctl(fd, I2C_SLAVE, dev_addr) < 0) {
		std::cout << "ioctl error" << std::endl;
		close(fd);
		return -1;
	}
	std::cout << "File " << i2cFile << " configured succesfully" << std::endl;
	return fd;
}

int I2CDevice::write(uint8_t dev_addr, uint8_t reg_addr, uint8_t value) 
{
	struct i2c_rdwr_ioctl_data packets;
	struct i2c_msg messages[2];
	uint8_t write_bytes[2];
		
	write_bytes[0] = reg_addr;
	write_bytes[1] = value;
	
	//Creamos el mensaje a enviar
	messages[0].addr	= dev_addr; // Device's address
	messages[0].flags	= 0;		// write
	messages[0].len		= sizeof(write_bytes);	// bytes in buffer
	messages[0].buf		= write_bytes;
	
	//Creamos el paquete a enviar
	packets.msgs  = messages;
	packets.nmsgs = SEND_MSG_SIZE;
	
	
	//Enviamos el paquete
	if (ioctl(fd, I2C_RDWR, &packets) < 0) {
		std::cout << "ioctl error" << std::endl;
		return 0;	// Error
	}
	
	return 1;
}

int I2CDevice::read(uint8_t addr, int fdI2C, uint8_t reg_addr, uint8_t *value)
{
	
	struct i2c_rdwr_ioctl_data packets; //
	struct i2c_msg messages[2];
	
	// Creamos el mensaje a enviar
	messages[0].addr	= addr; //Establecemos la dirección del esclavo I2C
	messages[0].flags	= 0;	// Establecemos la operacion de escritura
	messages[0].len		= R_LEN;	// Establecemos los bytes que queremos enviar
	messages[0].buf		= &reg_addr; //Establecemos los registros de los que queremos leer
	
	// Creamos el mensaje de lectura
	messages[1].addr	= addr; //Establecemos la dirección del esclavo I2C
	messages[1].flags	= I2C_M_RD;	//Establecemos la operacion de lectura
	messages[1].len		= R_LEN; //Establecemos los bytes que queremose enviar
	messages[1].buf		= value; //Esblecemos las variables donde se guardan los registros de los que queremos leer
	
	
	// Creamos el paquete a enviar
	packets.msgs  = messages; //Mensajes que se enviar
	packets.nmsgs = READ_MSG_SIZE; //Numero de mensajes que contiene el paquete
	
	// Enviamos el paquete
	if (ioctl(fdI2C, I2C_RDWR, &packets) < 0) {
		return 0;	// Error
	}
	
	
	return 1;
}

int I2CDevice::closeFD (void)
{
	if(close(fd)) {
		std::cout << "close error" << std::endl;
		return 0;
	}
	return 1;
}



