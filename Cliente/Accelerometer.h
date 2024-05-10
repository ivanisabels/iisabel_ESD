#ifndef ACCELEROMETER_H
	#define ACCELEROMETER_H

		#include "clientSend.h"
		#include "clientReceive.h"

		/*-------------- ADDRESS ------------------*/
		#define	 ACCELEROMETER_ADDR uint8_t(0x68)

		#define	 SMPRT_DIV uint8_t(0x19)
		#define	 CONFIG uint8_t(0x1A)
		#define	 ACCEL_CONFIG uint8_t(0x1C)
		#define	 INT_ENABLE uint8_t(0x38)	// Enables interrupt generation by interrupt sources
		#define	 ACCEL_XOUT_H_REG uint8_t(0x3B)	// [15:8]
		#define	 ACCEL_XOUT_L_REG uint8_t(0x3C)	// [7:0]
		#define	 ACCEL_YOUT_H_REG uint8_t(0x3D)
		#define	 ACCEL_YOUT_L_REG uint8_t(0x3E)
		#define	 ACCEL_ZOUT_H_REG uint8_t(0x3F)
		#define	 ACCEL_ZOUT_L_REG uint8_t(0x40)
		#define  PWR_MGMT_1 uint8_t(0x6B)
		#define  PWR_MGMT_2 uint8_t(0x6C)

		class Accelerometer {
			private:
				I2CDevice i2c;
			public:
				Accelerometer(void);
				virtual ~Accelerometer() {}	//Virtual destructor
				
				void	Accelerometer_measure(char *measures);
		};
#endif
