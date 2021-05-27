#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* Producer-consumer problem 
*/
#define MAX_ITEMS 10000 // aximum items a producer can produce or a consumer can consume

#define BUFFER_SIZE 1000 // Size of the buffer
#define NUMBER_PRODUCERS 100
#define NUMBER_CONSUMERS 100


pthread_cond_t non_full; /* can we add more elements? */
pthread_cond_t non_empty; /* can we remove elements? */
int n_elements; /* number of elements in buffer */
int in = 0;
int out = 0;
int buffer[BUFFER_SIZE];
pthread_mutex_t mutex;


void *producer(void *threadid)
{   
    int pno = (intptr_t) threadid;
    int item;
    srand(time(0));
    for(int i = 0; i < MAX_ITEMS/NUMBER_PRODUCERS; i++) {
		
        item = rand() % 100 + 1; // Produce an random item between 1 and 100
		
		pthread_mutex_lock(&mutex); /* access to buffer*/
		while (n_elements == BUFFER_SIZE) /* when buffer is full*/{
			printf("\tBuffer Full\n");
			pthread_cond_wait(&non_full, &mutex); 
		
		}

        buffer[in] = item;
		n_elements ++;
		
        printf("Producer %d: Insert Item %d at %d\n", pno,buffer[in],in);
        in = (in+1)%BUFFER_SIZE;
		
		pthread_cond_signal(&non_empty); /* buffer is not empty */
		pthread_mutex_unlock(&mutex);
    }
	
	return NULL;
}

void *consumer(void *threadid)
{   
    int cno = (intptr_t) threadid;
    for(int i = 0; i < MAX_ITEMS/NUMBER_CONSUMERS; i++) {
		pthread_mutex_lock(&mutex); /* access to buffer*/
		while (n_elements == 0) /* when buffer is empty*/{
			printf("\tBuffer tEmpty\n");
			pthread_cond_wait(&non_empty, &mutex); 
		}
        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n", cno,item, out);
        out = (out+1)%BUFFER_SIZE;
		n_elements --;		
		pthread_cond_signal(&non_full); /* buffer is not full */
		pthread_mutex_unlock(&mutex);
    }
	
	return NULL;
}

int main()
{   

    pthread_t pro[NUMBER_PRODUCERS],con[NUMBER_CONSUMERS];

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&non_full, NULL);
	pthread_cond_init(&non_empty, NULL);
	
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

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&non_full);
	pthread_cond_destroy(&non_empty);

    return 0;
    
}