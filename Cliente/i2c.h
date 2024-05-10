#ifndef I2C_H
	#define I2C_H
		#include <linux/i2c.h>
		#include <sys/ioctl.h> 		// Funcion ioctl
		#include <linux/i2c-dev.h>  	// Para I2C_SLAVE
		#include <fcntl.h> 		// Funcion open	
		#include <signal.h>
		#include <unistd.h> //usleep, close
		#include <cstdint> //uint_t
		#include <string>

		#define  SEND_MSG_SIZE 1	// Numero mensajes a enviar a MPU
		#define  READ_MSG_SIZE 2	// Número mensajes a leer de MPU
		#define  FILE_NAME "/dev/i2c-1"	// Archivo I2C: i2c-1
		#define  RGBC_ADDRESS 0x29 //TCS34725
		#define  W_LEN 2
		#define  R_LEN 2

				
		class I2CDevice {
			private:
				int fd;
				std::string devNum;
				uint8_t devAddr;
			public:
				I2CDevice(std::string devNum, uint8_t devAddr);
				virtual ~I2CDevice() {}	//Virtual destructor
				
				int		get_fd() const;
				std::string		get_devNum() const;
				int		get_devAddr() const;
				
				int		set_fd();
				
				int 	init(uint8_t device, uint8_t dev_addr) ;
				int 	write(uint8_t dev_addr, uint8_t reg_addr, uint8_t value) ;
				int 	read(uint8_t addr, uint8_t reg_addr, uint8_t *value) ;
				int 	closeFD (void) ;
				int     openFD (void);

		};
#endif
