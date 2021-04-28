#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const int MAX_NUMBER = 200;
int shared_data = 0;


enum numbers {EVEN, ODD};


void *oddThread(void *arg) {
   int counter=1;

   while (counter <=MAX_NUMBER){
      printf ("OddThread = %d\n", shared_data++);
      counter=counter+2;

   }
   pthread_exit(0);
}

void *evenThread(void *arg) {
   int counter=2;

   while (counter <=MAX_NUMBER){
      printf ("EvenThread = %d\n", shared_data++);
      counter=counter+2;


  }
   pthread_exit(0);
}

int main(){
    int i;
    pthread_t idth[2];


    pthread_create(&idth[0],NULL,oddThread,NULL);
    pthread_create(&idth[1],NULL,evenThread,NULL);
	
    // I wait for the completion of the thread
    for (i=0; i<2; i++) 
      pthread_join(idth[i],NULL);


    return(0);
}

