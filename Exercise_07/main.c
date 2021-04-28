#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* Producer-consumer problem 
*/
#define MAX_ITEMS 10000 // aximum items a producer can produce or a consumer can consume

#define BUFFER_SIZE 100 // Size of the buffer
#define NUMBER_PRODUCERS 1
#define NUMBER_CONSUMERS 1


int in = 0;
int out = 0;
int buffer[BUFFER_SIZE];


void *producer(void *threadid)
{   
    int pno = (intptr_t) threadid;
    int item;
    srand(time(0));
    for(int i = 0; i < MAX_ITEMS/NUMBER_PRODUCERS; i++) {
        item = rand() % 100 + 1; // Produce an random item between 1 and 100

        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", pno,buffer[in],in);
        in = (in+1)%BUFFER_SIZE;

    }
	
	
}

void *consumer(void *threadid)
{   
    int cno = (intptr_t) threadid;
    for(int i = 0; i < MAX_ITEMS/NUMBER_CONSUMERS; i++) {

        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n", cno,item, out);
        out = (out+1)%BUFFER_SIZE;

    }
}

int main()
{   

    pthread_t pro[NUMBER_PRODUCERS],con[NUMBER_CONSUMERS];


    for(int i = 0; i < NUMBER_PRODUCERS; i++) {

        pthread_create(&pro[i], NULL, (void *)producer, (void *) (intptr_t) i+1);
    }
    for(int i = 0; i < NUMBER_CONSUMERS; i++) {
 
        pthread_create(&con[i], NULL, (void *)consumer, (void *) (intptr_t) i+1);
    }

    for(int i = 0; i < NUMBER_PRODUCERS; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < NUMBER_CONSUMERS; i++) {
        pthread_join(con[i], NULL);
    }


    return 0;
    
}