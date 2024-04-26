#include <linux/i2c.h>
#include <sys/ioctl.h> 		// Funcion ioctl
#include <linux/i2c-dev.h>  	// Para I2C_SLAVE
#include <fcntl.h> 		// Funcion open	
#include <signal.h>
#include <unistd.h> //usleep, close

#define  SEND_MSG_SIZE 1	// Numero mensajes a enviar a MPU
#define  READ_MSG_SIZE 2	// Número mensajes a leer de MPU
#define  FILE_NAME "/dev/i2c-1"	// Archivo I2C: i2c-1
#define	 MPU_ADDR 0x68
#define RGBC_ADDRESS 0x29 //TCS34725
#define  W_LEN 2
#define  R_LEN 2

		
class I2CDevice {
	private:
		int fd;
		uint8_t devNum;
		uint8_t devAddr;
	public:
		I2CDevice(uint8_t devNum, uint8_t devAddr);
		virtual ~I2CDevice() {}	//Virtual destructor
		
		int 	init(uint8_t device, uint8_t dev_addr) ;
		int 	write(uint8_t dev_addr, uint8_t reg_addr, uint8_t value) ;
		int 	read(uint8_t addr, int fdI2C, uint8_t reg_addr, uint8_t *value) ;
		int 	closeFD (void) ;
};

