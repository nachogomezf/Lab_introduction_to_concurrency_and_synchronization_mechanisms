#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
int dato_compartido = 0;
int es_par = 0;
pthread_mutex_t m;
pthread_cond_t cL, cV;

void* pares (){
	int i=0;
	while (i<100){
		//printf("Hello from thread 1");
		pthread_mutex_lock(&m);
		while (!(es_par)){
			pthread_cond_wait(&cL,&m);
		}
		printf("Thread2=%d\n",dato_compartido);
		dato_compartido++;
		es_par=0;
		pthread_cond_signal(&cV);
		pthread_mutex_unlock(&m);
		i++;
	}
	pthread_exit(0);
	
}

void* impares (){
	int j=0;
	while (j<100){
		//printf("Hello from thread 2");
		pthread_mutex_lock(&m);
		while (es_par){
			pthread_cond_wait(&cV,&m);
		}
		printf("Thread1=%d\n",dato_compartido);
		dato_compartido++;
		es_par=1;
		pthread_cond_signal(&cL);
		pthread_mutex_unlock(&m);
		j++;
	}
	pthread_exit(0);
}
int main(void) {
printf("hola1");
pthread_t th1, th2;
pthread_mutex_init(&m, NULL);
pthread_cond_init(&cL, NULL);
pthread_cond_init(&cV, NULL);
printf("hola2");
pthread_create(&th1, NULL, &pares, NULL);
printf("hola3");
pthread_create(&th2, NULL, &impares, NULL);
printf("hola4");
pthread_join(th1, NULL);
pthread_join(th2, NULL);
pthread_mutex_destroy(&m);
pthread_cond_destroy(&cL);
pthread_cond_destroy(&cV);
}