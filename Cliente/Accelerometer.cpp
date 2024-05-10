#include "Accelerometer.h"

Accelerometer::Accelerometer(void)
    : i2c(1, ACCELEROMETER_ADDR) //Initialice the I2C interface
{
	std::cout << "INTIALIZACION CONFIGURATION ACCELEROMETER" << std::endl;
	
	// Configuration Power management 1
	int device_reset = 0; // [7] Resets all internal registers to their default value (does not work if enabled)
	int sleep = 0;	// [6] Low power mode
	int cycle = 1;	// [5] Cycle between sleep and waking up to take a single sample of data from accel at LP_WAKE_CTRL rate (sleep must be 0)
	int temp_dis = 1;	// [3] Disables temp sensor
	int clksel = 0; // [2:0] 0 = internal 8MHz oscillator Activamos el oscilador interno de 8 MHz
	uint8_t pwr_mgmt_1 = (device_reset << 7) | (sleep << 6) | (cycle << 5) | (temp_dis << 3) | (clksel);
	//Send the configuration to the I2C interface
	if ( i2c.write(ACCELEROMETER_ADDR, PWR_MGMT_1, pwr_mgmt_1) == -1) {
		std::cout << "Error Configuration Accelerometer: Power Management 1" << std::endl;
	}
	
	
	//Configuration Power management 2
	uint8_t lp_wake_ctrl = 3 << 6; // [7:6] Wake-up frequency = 40Hz = 25ms
	int stby_xa = 0 << 5;	// [5] Low power mode Axis X Accelerometer
	int stby_ya = 0 << 4;	// [4] Low power mode Axis Y Accelerometer
	int stby_za = 0 << 3;	// [3] Low power mode Axis Z Accelerometer
	int stby_xg = 1 << 2;	// [2] Low power mode Axis X Gyroscope
	int stby_yg = 1 << 1;	// [1] Low power mode Axis Y Gyroscope
	int stby_zg = 1;	// [0] Low power mode Axis Z Gyroscope
	uint8_t pwr_mgmt_2 = (lp_wake_ctrl) | (stby_xa) | (stby_ya) | (stby_za) | (stby_xg) | (stby_yg) | (stby_zg);
	//Send the configuration to the I2C interface
	if ( i2c.write(ACCELEROMETER_ADDR, PWR_MGMT_2, pwr_mgmt_2) == -1) {
		std::cout << "Error Configuration Accelerometer: Power Management 2" << std::endl;
	}
	
	
	//Specify the sampling rate
	uint8_t smplrt_div = 7;	
	//Send the configuration to the I2C interface
	if ( i2c.write(ACCELEROMETER_ADDR, SMPRT_DIV, smplrt_div) == -1) {
		std::cout << "Error Configuration Accelerometer: Sampling Rate" << std::endl;
	}
	
	
	//Set the bandwidth of the digital low pass filter
	uint8_t dlpf_cfg = 3;	
	//Send the configuration to the I2C interface
	if ( i2c.write(ACCELEROMETER_ADDR, CONFIG, dlpf_cfg)  == -1) {
		std::cout << "Error Configuration Accelerometer: Bandwidth of the Digital Low Pass Filter" << std::endl;
	}
	
	
	//Selects the full scale range of the accelerometer outputs	
	uint8_t afs_sel = 0;	
	//Send the configuration to the I2C interface
	if ( i2c.write(ACCELEROMETER_ADDR, ACCEL_CONFIG, afs_sel)  == -1) {
		std::cout << "Error Configuration Accelerometer: Scale Range" << std::endl;
	}
	
	//Activates the interruption of the read value, which occurs every time a write operation is completed (the values that we request to read from a register have been written)
	uint8_t data_rdy_en = 1;	
	//Send the configuration to the I2C interface
	if ( i2c.write(ACCELEROMETER_ADDR, INT_ENABLE, data_rdy_en)  == -1) {
		std::cout << "Error Configuration Accelerometer: Interruption Activation" << std::endl;
	}
	
	std::cout << "	CORRECT" << std::endl << std::endl;
	
	i2c.closeFD();
	
}

void Accelerometer::Accelerometer_measure( char *measures){
	uint8_t registrosAcelerometro[] = {ACCEL_XOUT_H_REG, ACCEL_XOUT_L_REG, ACCEL_YOUT_H_REG, ACCEL_YOUT_L_REG, ACCEL_ZOUT_H_REG, ACCEL_ZOUT_L_REG};
	uint8_t readingAccelerometerRegisters[6];
	
	//Open file descriptor
	i2c.openFD();
	
	//Read the accelerometer records
	for(int n=0; n<6; n++){
		if( i2c.read(ACCELEROMETER_ADDR, registrosAcelerometro[n], &readingAccelerometerRegisters[n]) == -1){
			std::cout << "Error Reading Measures Accelerometer" << std::endl;
		}
	}
	
	for(int n=0; n<6; n++){
		measures[n] = readingAccelerometerRegisters[n];
	}
	
	i2c.closeFD();

}
