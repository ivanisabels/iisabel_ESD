#include "I2C.h"
#include <signal.h>		// Para las señales que se puedan recibir
#include <math.h>

#define PWR_CONFIG 0x80
#define POWER_ON 0x01
#define POWER_OFF 0x00
#define RGBC_MEASURE 0x03

#define RGBC_ADDRESS 0x29 //TCS34725

#define CLEAR_L 0x94
#define CLEAR_H 0x95
#define RED_L 0x96
#define RED_H 0x97
#define GREEN_L 0x98
#define GREEN_H 0x99
#define BLUE_L 0x9A
#define BLUE_H 0x9B

//Color RGB
typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
} ColorRGB;

//Color
typedef struct {
    char* nombre;
    ColorRGB rgb;
} Color;

//Table of colors closest 
Color colores[] = {
        {"Red", {255, 0, 0}},
        {"Green", {0, 255, 0}},
        {"Blue", {0, 0, 255}},
        {"Yellow", {255, 255, 0}},
        {"Purple", {128, 0, 128}},
        {"Cyan", {0, 255, 255}},
        {"Orange", {255, 165, 0}},
        {"Brown", {139, 69, 19}},
        {"White", {255, 255, 255}},
        {"Black", {0, 0, 0}},
};

uint8_t final = 1;
static void RGBC_Init(void);
static void RGBC_meas(void);
static void controlarRupturaPrograma(void);
static void cerrarPrograma(int sig);

int main(void){
	
	controlarRupturaPrograma();
	
	//Iniciamos el bus I2C
	init_i2c(1, RGBC_ADDRESS);
	
	//Init de RGBC
	RGBC_Init();
	
	while(final){
		//Clear the terminal
		system("clear");
		
		//Do the measures
		RGBC_meas();
		
		usleep(500000);
	}
	
	return 0;
	
}



//Function to calculate the Euclidean distance between two colors
static double distancia(ColorRGB c1, ColorRGB c2) {
    return sqrt( pow(c1.r - c2.r, 2) + pow(c1.g - c2.g, 2) + pow(c1.b - c2.b, 2) );
}

//Function to find the closest color in the color table
static Color buscarColorMasCercano(ColorRGB color) {
    Color colorMasCercano = colores[0];
    double distanciaMinima = distancia(color, colores[0].rgb);

    for (uint8_t i = 1; i < 10; ++i) {
        double dist = distancia(color, colores[i].rgb);
        if (dist < distanciaMinima) {
            distanciaMinima = dist;
            colorMasCercano = colores[i];
        }
    }

    return colorMasCercano;
}

static void RGBC_Init(void){
	
	//Init the TSC3472 
	i2c_write( RGBC_ADDRESS,fdI2C, PWR_CONFIG, POWER_ON);
	
	//Wait 2.4 ms
	usleep(24000);
	
}

static void RGBC_meas(void){
	
	uint8_t clear_low, clear_high;
	uint8_t red_low, red_high;
	uint8_t green_low, green_high;
	uint8_t blue_low, blue_high;
	uint16_t clear, red, blue, green;
	ColorRGB color_Medido;
	Color color_Cercano;
	
	//Enable the TSC3472 to do a measure
	i2c_write( RGBC_ADDRESS,fdI2C, PWR_CONFIG , RGBC_MEASURE);
	
	//Wait until measure is done
	usleep(4800);
	
	//Read the measure CLEAR
	i2c_read(RGBC_ADDRESS,fdI2C, CLEAR_L, &clear_low);
	i2c_read(RGBC_ADDRESS,fdI2C, CLEAR_H, &clear_high);
	clear = (clear_high << 8) | clear_low;
	printf("CLEAR: %d\n", clear);
	
	//Read the measure RED
	i2c_read(RGBC_ADDRESS,fdI2C, RED_L, 	&red_low);
	i2c_read(RGBC_ADDRESS,fdI2C, RED_H, 	&red_high);
	red = (red_high << 8) | red_low;
	red = (red * 255) / 65535;
	printf("RED: %d\n", red);
	color_Medido.r = red;
	
	//Read the measure BLUE
	i2c_read(RGBC_ADDRESS,fdI2C, BLUE_L, &blue_low);
	i2c_read(RGBC_ADDRESS,fdI2C, BLUE_H, &blue_high);
	blue = (blue_high << 8) | blue_low;
	blue = (blue * 255) / 65535;
	printf("BLUE: %d\n", blue);
	color_Medido.b = blue;
	
	//Read the measure GREEN
	i2c_read(RGBC_ADDRESS,fdI2C, GREEN_L, &green_low); 
	i2c_read(RGBC_ADDRESS,fdI2C, GREEN_H, &green_high);
	green = (green_high << 8) | green_low;
	green = (green * 255) / 65535;
	printf("GREEN: %d\n", green);
	color_Medido.g = green;

	// Calculate luminance
	float luminance = (-0.32466) * (red) + (1.57837) * (green) + (-0.73191) * (blue);
	printf("LUMINANCE: %f\n" , luminance);
	
	//Calculate the closest color to the one that corresponds
	color_Cercano = buscarColorMasCercano(color_Medido);
	printf("COLOR MAS CERCANO: %s\n" , color_Cercano.nombre);
}

static void controlarRupturaPrograma(void){
	// Estructura para especificar el comportamiento de sigaction
    struct sigaction sa;

    // Asignar la función de manejador de señales 
    sa.sa_handler = cerrarPrograma;
    
    // Limpiar el conjunto de señales a las que se aplica sa_mask
    sigemptyset(&sa.sa_mask);
    
    // Restaurar el comportamiento predeterminado de las señales
    sa.sa_flags = 0;
    
    //Controlamos el programa para cuando se envian señales para cerrar el programa
    sigaction(SIGINT, &sa, NULL); //Para cuando se le  envía al programa un Control+C
    sigaction(SIGTSTP, &sa, NULL);  //Para cuando se le  envía al programa un Control+Z

}

static void cerrarPrograma(int sig){
    final = 0;
}
