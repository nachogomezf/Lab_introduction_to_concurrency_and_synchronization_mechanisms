#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

void* siesta(void* arg)

{    
    
    printf("\n First..\n");
    
    sleep(4);    
    
    printf("\n Second..\n");
    
}


int main()

{


	pthread_t t1,t2;

	pthread_create(&t1,NULL,siesta,NULL);

	sleep(2);

	pthread_create(&t2,NULL,siesta,NULL);

	pthread_join(t1,NULL);

	pthread_join(t2,NULL);


	return 0;

}
