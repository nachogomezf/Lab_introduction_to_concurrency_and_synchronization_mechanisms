/* compile as "gcc -pthread -o main.c main" */
/* execute as "taskset 0x01 main" to run on a single processor */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMBER_ADDED 10000
#define NUMBER_TIMES 10000


sem_t semaphore;

/** 
* Global variable with the accumulated value where the sum is made.
* Acts as a shared variable between the two threads.
*/
long total_amount = 0;

/**
* function that returns accumulator + n. Makes the
* sum one by one with a for loop.
*/
long addN(long accumulator, int n) {
    int i;
    long total = accumulator;
    
    for (i=0;i<n;i++){
       total += 1;
        
    }
    
    return total;
}

/**
* The function to be executed through a thread. Call
* NUMBER_TIMES addN function to add NUMBER_ADDED to the total_amount variable.
*/
void run() {
    int i;
    
    for (i=1;i<= NUMBER_TIMES;i++){
		 //wait
		sem_wait(&semaphore);
		
		//critical section
        total_amount = addN (total_amount, NUMBER_ADDED);
		 
		 //signal        
		sem_post(&semaphore);
	
    }
   
   
}

/* Main program. Runs simultaneously on two different threads.
    Wait for both to finish and look at the final summed value, 
    checking if it is the expected.
*/
     
int main() {
    
    
    pthread_t th1, th2;
    long expected_result;
	
	sem_init(&semaphore, 0, 1);
	
    // creamos 2 hilos
    pthread_create(&th1,NULL,(void*)run, NULL);
    pthread_create(&th2,NULL,(void*)run, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    expected_result = NUMBER_ADDED * NUMBER_TIMES * 2;
   
    printf("The final result is %ld\n" ,total_amount);
    printf("We expected %ld\n" , expected_result);

        if (total_amount != expected_result)
            printf("DO NOT MATCH!!!\n");
        
	sem_destroy(&semaphore);
	
} 
