#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t mut;
pthread_cond_t cruza_izq, cruza_dch;
int izq=0, dch =0;

void* coche_izq(void* arg){
	int id =*((int*)arg);
	printf("LLegando coche a orilla izq: %d\n",id);
	pthread_mutex_lock(&mut);
	while (dch>0){
		pthread_cond_wait(&cruza_izq,&mut);
	}
	printf("Voy a cruzar el puente hacia orilla derecha: %d\n",id);
	izq++;
	pthread_mutex_unlock(&mut);
	pthread_cond_broadcast(&cruza_izq);
	sleep(5);

	
	printf("Ya he cruzado el puente hacia orilla derecha: %d\n",id);
	izq--;
	if (izq==0) pthread_cond_broadcast(&cruza_dch);

	//pthread_cond_broadcast(&cruza_izq);
	pthread_exit(NULL);
}

void* coche_dch(void* arg){
	int id =*((int*)arg);
	printf("LLegando coche a orilla dch: %d\n",id);
	pthread_mutex_lock(&mut);
	while (izq>0){
		pthread_cond_wait(&cruza_dch,&mut);
	}
	printf("Voy a cruzar el puente hacia orilla izquierda: %d\n",id);
	dch++;
	pthread_mutex_unlock(&mut);
	pthread_cond_broadcast(&cruza_dch);
	sleep(2);
	printf("Ya he cruzado el puente hacia orilla izquierda: %d\n",id);
	dch--;
	pthread_cond_broadcast(&cruza_izq);
	//pthread_cond_broadcast(&cruza_dch);
	pthread_exit(NULL);
}

int main(){
	pthread_t coches_d[10], coches_i[10];
	int coches[10];
	for (int i=0; i<10; i++){
		coches[i]=i;
		pthread_create(&coches_i[i],NULL,(void*)coche_izq,&coches[i]);
	}
	for (int i=0; i<10; i++){
		coches[i]=i;
		pthread_create(&coches_d[i],NULL,(void*)coche_dch,&coches[i]);
	}
	for (int i=0; i<10; i++){
		pthread_join(coches_i[i],NULL);
	}
	for (int i=0; i<10; i++){
		pthread_join(coches_d[i],NULL);
	}
	pthread_mutex_destroy(&mut);
	pthread_cond_destroy(&cruza_dch);
	pthread_cond_destroy(&cruza_izq);
	exit(0);
}