#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaphores[5],isFinished;

void Write(char* mensaje){
	printf("%s",mensaje);
	int delay;
	srand(time(NULL));
	delay=(rand()%4);
	sleep(delay);
}

void* ra(){
	Write("a1\n");
	sem_post(&semaphores[0]);

	sem_wait(&semaphores[3]);
	Write("a2\n");
	sem_post(&semaphores[1]);
	sem_post(&semaphores[3]);

	sem_wait(&semaphores[2]);
	Write("a3\n");
}
void* rb(){
	Write("b1\n");
	sem_post(&semaphores[3]);

	Write("b2\n");
	sem_post(&semaphores[4]);

	sem_wait(&semaphores[1]);
	Write("b3\n");
	sem_post(&semaphores[1]);
	if (semaphores[0]==semaphores[1]==semaphores[2]==semaphores[3]==semaphores[4]==1){
		sem_post(&isFinsihed);
	}
	while (semaphores[0]!=semaphores[1]!=semaphores[2]!=semaphores[3]!=semaphores[4]!=1){
		if (semaphores[0]==semaphores[1]==semaphores[2]==semaphores[3]==semaphores[4]==1){
			sem_post(&isFinsihed);
		}
		sem_trywait(&isFinished);
	}
}
void* rc(){
	sem_wait(&semaphores[0]);
	Write("c1\n");
	sem_post(&semaphores[0]);

	sem_wait(&semaphores[4]);
	Write("c2\n");
	sem_post(&semaphores[4]);
	sem_post(&semaphores[2]);

	Write("c3\n");
}

int main()

{	for (int i=0; i<5; i++){
		sem_init(&semaphores[i],0,0);
	}
	pthread_t threads[3];
	pthread_create(&threads[0],NULL,ra,NULL);
	pthread_create(&threads[1],NULL,rb,NULL);
	pthread_create(&threads[2],NULL,rc,NULL);
	for (int i=0; i<3; i++){
		pthread_join(threads[i],NULL);
	}
	for (int i=0; i<5; i++){
		sem_destroy(&semaphores[i]);
	}

	return 0;

}