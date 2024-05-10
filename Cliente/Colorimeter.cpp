#include "Colorimeter.h"

Colorimeter::Colorimeter(void) 
	: i2c("1", COLORIMETER_ADDRESS) //Initialice the I2C interface
{

	std::cout << "INTIALIZACION CONFIGURATION COLORIMETER" << std::endl;
	
    //Init the TSC3472 
	if( i2c.write( COLORIMETER_ADDRESS, PWR_CONFIG, POWER_ON) == -1){
		std::cout << "Error Initialization Colorimeter " << std::endl;
	}
	
	std::cout << "	CORRECT" << std::endl << std::endl; 
	
	//Wait 2.4 ms
	usleep(24000);
	
	i2c.closeFD();
}

void Colorimeter::Colorimeter_measure(char *measures){
	
	uint8_t clear_low, clear_high;
	uint8_t red_low, red_high;
	uint8_t green_low, green_high;
	uint8_t blue_low, blue_high;
	
	i2c.openFD();
		
	//Enable the TSC3472 to do a measure
	if( i2c.write( COLORIMETER_ADDRESS, PWR_CONFIG , COLORIMETER_MEASURE) == -1){
		std::cout << "Error Reading Measures Colorimeter" << std::endl;
	}
	
	//Wait until measure is done
	usleep(4800);
	
	//Read the measure CLEAR
	i2c.read( COLORIMETER_ADDRESS,CLEAR_L, &clear_low);
	measures[1] = clear_low;
	i2c.read( COLORIMETER_ADDRESS,CLEAR_H, &clear_high);
	measures[0] = clear_high;
	
	//Read the measure RED
	i2c.read(COLORIMETER_ADDRESS,RED_L, &red_low);
	measures[3] = red_low;
	i2c.read(COLORIMETER_ADDRESS,RED_H, &red_high);
	measures[2] = red_high;
	
	//Read the measure BLUE
	i2c.read(COLORIMETER_ADDRESS,BLUE_L, &blue_low);
	measures[5] = blue_low;
	i2c.read(COLORIMETER_ADDRESS,BLUE_H, &blue_high);
	measures[4] = blue_high;
	
	//Read the measure GREEN
	i2c.read(COLORIMETER_ADDRESS,GREEN_L, &green_low); 
	measures[7] = green_low;
	i2c.read(COLORIMETER_ADDRESS,GREEN_H, &green_high);
	measures[6] = green_high;
	
	i2c.closeFD();
}
