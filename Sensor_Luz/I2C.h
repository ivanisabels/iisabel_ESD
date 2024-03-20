#ifndef I2C_H
	#define I2C_H
		#include <stdio.h>
		#include <stdint.h>	// uintx_t
		#include <linux/i2c.h>
		#include <sys/ioctl.h> 		// Funcion ioctl
		#include <linux/i2c-dev.h>  	// Para I2C_SLAVE
		#include <string.h>
		#include <fcntl.h> 		// Funcion open	
		#include <stdlib.h> //system(clear)
		#include <pthread.h> //Para hilos
		#include <signal.h>
		#include <unistd.h> //usleep, close

		#define  SEND_MSG_SIZE 1	// Numero mensajes a enviar a MPU
		#define  READ_MSG_SIZE 2	// Número mensajes a leer de MPU
		#define  FILE_NAME "/dev/i2c-1"	// Archivo I2C: i2c-1
		#define	 MPU_ADDR 0x68
		#define RGBC_ADDRESS 0x29 //TCS34725
		#define  W_LEN 2
		#define  R_LEN 2

		extern int fdI2C; //Fichero descriptor para leer

		void init_i2c( int device, int addr);
		int i2c_write(uint8_t addr,int fdI2C, uint8_t reg_addr, uint8_t value);
		int i2c_read(uint8_t addr, int fdI2C, uint8_t reg_addr, uint8_t* value);
#endif
