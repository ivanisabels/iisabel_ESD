#include "Acelerometro.h"
#include <stdio.h>
#include <pthread.h>
#include <signal.h>		// Para las señales que se puedan recibir
#include <unistd.h>     // Para close


static void cerrarPrograma(int sig);
static void controlarRupturaPrograma(void);

int main(int argc, char **argv) {
	
	
	printf("INICIO PROGRAMA\n");
	
	controlarRupturaPrograma();
	
	//Creamos el hilo del I2C
	pthread_create (&hAcelerometro , NULL , acelerometro , NULL );
	
	//Esperamos a que el hilo termine del I2C
	pthread_join ( hAcelerometro , NULL ) ;
	
	printf("\nFINAL PROGRAMA\n");
	
	return 0;
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
    //Eliminamos el hilo que obtiene las medidas del acelerometro
    pthread_kill(hAcelerometro, SIGTERM);
}
