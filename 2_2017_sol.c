#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX 100
pthread_mutex_t mtxPuente;
pthread_cond_t cndIzq, cndDer;
int cruzando_izq = 0, cruzando_der = 0;
void *coche_izq(void *ptr){
int id;
id = *((int*)ptr);
printf("Llega


pthread_mutex_lock(&mtxPuente);
while( cruzando_der > 0 )
pthread_cond_wait(&cndIzq,
&mtxPuente);
cruzando_izq++;
pthread_mutex_unlock(&mtxPuente);
printf("Cruzo hacia la derecha:
%d\n",id);
sleep(2);
pthread_mutex_lock(&mtxPuente);
cruzando_izq--;
if( cruzando_izq == 0 )
pthread_cond_broadcast(&cndDer);
pthread_mutex_unlock(&mtxPuente);
printf("Ya he cruzado a derecha:
%d\n", id);
pthread_exit(NULL);
}
void *coche_der(void *ptr){
int id;
id = *((int*)ptr);
printf("Llega coche a derecha:
%d\n", id);
pthread_mutex_lock(&mtxPuente);
while( cruzando_izq > 0 )
pthread_cond_wait(&cndDer,
&mtxPuente);
cruzando_der++;
pthread_mutex_unlock(&mtxPuente);
printf("Cruzo hacia la
izquierda: %d\n", id);
sleep(2);
pthread_mutex_lock(&mtxPuente);
cruzando_der--;
if( cruzando_der == 0 )
pthread_cond_broadcast(&cndIzq);
pthread_mutex_unlock(&mtxPuente);
printf("Ya he cruzado a
izquierda: %d\n", id);
pthread_exit(NULL);
}
int main(int argc, char *argv[]){
int i, num_coches_izq, num_coches_der, coches_id[MAX];
pthread_t coches[MAX];
num_coches_izq = 10;
num_coches_der = 10;
pthread_mutex_init(&mtxPuente, NULL);
pthread_cond_init(&cndIzq, NULL);
pthread_cond_init(&cndDer, NULL);
for( i = 0; i < num_coches_izq; i++ ){
coches_id[i] = i;
pthread_create(&coches[i], NULL, coche_izq, &coches_id[i]) :
}
for( i = 0; i < num_coches_der; i++ ){
coches_id[i] = i;
pthread_create(&coches[i], NULL, coche_der, &coches_id[i]);
}
for( i = 0; i < num_coches_izq + num_coches_der; i++ )
pthread_join(coches[i], NULL);
pthread_mutex_destroy(&mtxPuente);
pthread_cond_destroy(&cndIzq);
pthread_cond_destroy(&cndDer);
exit(0);
}