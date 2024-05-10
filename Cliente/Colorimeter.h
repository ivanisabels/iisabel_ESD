#ifndef COLORIMETER_H
	#define COLORIMETER_H
		
		#include "clientSend.h"
		#include "clientReceive.h"

		/*-------------- ADDRESS ------------------*/ 
		#define COLORIMETER_ADDRESS uint8_t(0x29)

		/*-------------- CONFIGURATION REGISTER ------------------*/ 
		#define PWR_CONFIG uint8_t(0x80)
		#define POWER_ON uint8_t(0x01)
		#define POWER_OFF uint8_t(0x00)
		#define COLORIMETER_MEASURE uint8_t(0x03)

		/*-------------- MEASURE REGISTER ------------------*/
		#define CLEAR_L uint8_t(0x94)
		#define CLEAR_H uint8_t(0x95)
		#define RED_L uint8_t(0x96)
		#define RED_H uint8_t(0x97)
		#define GREEN_L uint8_t(0x98)
		#define GREEN_H uint8_t(0x99)
		#define BLUE_L uint8_t(0x9A)
		#define BLUE_H uint8_t(0x9B)

		class Colorimeter {
			private:
				I2CDevice i2c;
			public:
				Colorimeter(void);
				virtual ~Colorimeter() {}	//Virtual destructor
				
				void	Colorimeter_measure(uint16_t *measures);
		};
#endif
