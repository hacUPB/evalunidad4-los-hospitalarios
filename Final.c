#include <stdio.h>
#include <pthread.h>  
#include<semaphore.h>
#define NUMITERATIONS 5
#define NUMITERATIONSE 5

sem_t empty,full;               // Definir el semáforo de sincronización global vacío, lleno
pthread_mutex_t mutex;          // Definir un mutex global, en diferentes funciones  
int buffer_count=0;   
char diagnostico; // Defina una variable global para indicar el número de productos en la tubería  

void *enfermos( void *arg );     // hilo productor  
void *medicos( void *arg );    // Hilo del consumidor  

int main(int argc , char *argv[]){  
    pthread_t thrd_prod , thrd_cons;  

    pthread_mutex_init( &mutex , NULL );    // Inicializa el mutex  
    
    sem_init (&empty, 0, 6);            // Inicializa el semáforo vacío
    sem_init (&full, 0, 0);         // Inicializa el semáforo completo
    // Crear hilos de productor y consumidor  
    if( pthread_create( &thrd_prod , NULL, enfermos ,  
                NULL ) != 0 )  
        printf( "thread create failed." );

    if( pthread_create( &thrd_cons , NULL, medicos ,  
                NULL ) != 0 )  
        printf( "thread create failed." );  

    // Espera al final del hilo  
    if( pthread_join( thrd_prod , NULL ) != 0 )  
        printf( " wait thread failed.");  
    if( pthread_join( thrd_cons , NULL ) != 0 )  
        printf( " wait thread failed.");  
    sem_destroy (&full);            // Liberar la cantidad de sincronización
    sem_destroy(&empty);            // Liberar la cantidad de sincronización
    pthread_mutex_destroy( &mutex );        // Cerrar el mutex  
    return 0;  
}  

void *enfermos( void *arg){  
    while(1){  
        sem_wait(&empty);  //empty-1
        for (size_t i = 0; i < NUMITERATIONSE; i++)
	    {
            pthread_mutex_lock(&mutex);//Bloquear
            printf( "el enfermo llega al Hospital con un diagnostico de ");
            scanf("%s", &diagnostico);
            buffer_count++; // Ocupa con éxito el mutex, y luego puede producir el búfer (almacén)
            printf(" es tranladado a la cama %d\n",buffer_count) ;//Operación
            pthread_mutex_unlock(&mutex);//Desbloquear
	    }     
        sem_post(&full);        //full+1
        return NULL;
    }  
}  

void *medicos( void *arg ){  
    while(1)
        {
        sem_wait(&full); //full-1
        for (size_t i = 0; i < NUMITERATIONS; i++)
	    {
            pthread_mutex_lock(&mutex);//Bloquear
            printf( " el medico atiende al enfermo ");  
            buffer_count--; // Ocupa con éxito el mutex, y luego puedes sacar el búfer (almacén)
            printf(" de la cama %d\n",buffer_count) ; //Operación
            pthread_mutex_unlock(&mutex); //Desbloquear
	    }       
        sem_post(&empty);        //empty-1
        return NULL;
    }
}
