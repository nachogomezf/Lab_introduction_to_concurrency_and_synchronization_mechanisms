#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const int MAX_NUMBER = 200;
int shared_data = 0;

pthread_mutex_t mutex;
pthread_cond_t waitEven;
pthread_cond_t waitOdd;

enum numbers {EVEN, ODD};
enum numbers turn= EVEN;


void *oddThread(void *arg) {
   int counter=1;

   while (counter <=MAX_NUMBER){
	  pthread_mutex_lock (&mutex);

	  while (turn!=ODD) {
         pthread_cond_wait(&waitEven, &mutex);
	  }
	  
      printf ("OddThread = %d\n", shared_data++);
      counter=counter+2;
	  
	  turn=EVEN;
	  pthread_cond_signal(&waitOdd);
      pthread_mutex_unlock (&mutex);
   }
   pthread_exit(0);
}

void *evenThread(void *arg) {
   int counter=2;   

   while (counter <=MAX_NUMBER){
	   
	  pthread_mutex_lock (&mutex);

	  while (turn!=EVEN) {
         pthread_cond_wait(&waitOdd, &mutex);
	  }
	  
      printf ("EvenThread = %d\n", shared_data++);
      counter=counter+2;
	  
	  turn=ODD;
	  pthread_cond_signal(&waitEven);
      pthread_mutex_unlock (&mutex);

  }
   pthread_exit(0);
}

int main(){
    int i;
    pthread_t idth[2];

	pthread_mutex_init (&mutex, NULL);
    pthread_cond_init (&waitEven, NULL);
    pthread_cond_init (&waitOdd, NULL);

    pthread_create(&idth[0],NULL,oddThread,NULL);
    pthread_create(&idth[1],NULL,evenThread,NULL);
	
    // I wait for the completion of the thread
    for (i=0; i<2; i++) 
      pthread_join(idth[i],NULL);

	pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&waitEven);
    pthread_cond_destroy(&waitOdd);

    return(0);
}

