#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
 
sem_t semwrite, semread;
//these two semaphores will help in synchronisation, when read finishes it will
//release read semaphore and vice versa
int count=-1;
void * writethread(void *args) {

   for(int i=0;i<=20;i++){
        sem_wait(&semwrite); //aquire the write semaphore
        //sleep(1);
        count++;
        printf("Count is: %d and incrementing thread id: %d\n", count,gettid());
        sem_post(&semread); //release the read semphore, now read thread can operate
    }
    
}
 
void * readthread(void *args) {
   
        
   for(int i=0;i<=20;i++) {
        sem_wait(&semread);//aquire the read semaphore
        //sleep(2);
        printf("Count is: %d and reading thread id:      %d \n\n",count ,gettid());
        sem_post(&semwrite);//release the write semphore so that the next write can take place
    }
   
}

int main() {
     pthread_t thread[2];
     sem_init(&semwrite, 0, 1);//first operation is to write hence the init value is 1
     sem_init(&semread, 0, 0);
 
     pthread_create(&thread[0], NULL, writethread, NULL);
     pthread_create(&thread[1], NULL, readthread, NULL);
     
     
     pthread_join(thread[0], NULL);
     pthread_join(thread[1], NULL);

     return 0;
}