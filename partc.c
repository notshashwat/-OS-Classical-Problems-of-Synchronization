
#include <pthread.h>
#include <stdio.h>

#include <semaphore.h>



sem_t writers_smph;
int writer_count = 0;
int reader_count = 0;
int shared_variable = 10;
pthread_mutex_t mutex;

void *writer(void *args)
{   
    sem_wait(&writers_smph);//aquire the writer semaphore
    writer_count++;
    printf("Writer count : %d\n",writer_count);
    shared_variable++;//incrementing the shared variable
    printf("Writer is writing the variable : %d\n",shared_variable);
    writer_count--;
    if(writer_count==0){
        printf("Writer count : %d\n",writer_count);
    }
    sem_post(&writers_smph);//give up the writer semaphore

}


void *reader(void *rid)
{   
    
    pthread_mutex_lock(&mutex);
    reader_count++;//increase the number of readers
    if(reader_count == 1) {
        sem_wait(&writers_smph); //the first reader aquires the writer semaphore
    }
    printf("Reader count %d\n", reader_count);
    pthread_mutex_unlock(&mutex);
    printf("Reader %d: reading the variable: %d\n",*((int *)rid),shared_variable);//any number of readers can read the shared variable at the same time

    pthread_mutex_lock(&mutex);
   
    printf("Reader %d left\n",*((int *)rid));

    reader_count--;//decrement the number of readers as the reader leaves
    if(reader_count == 0) {
        sem_post(&writers_smph);//the last reader gives up the writer semaphore, so that writer can aquire it
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{   

    pthread_mutex_init(&mutex, NULL);
    pthread_t read_thread[10],write_thread[3];
    sem_init(&writers_smph,0,1);

    int reader_id[10] = {1,2,3,4,5,6,7,8,9,10}; //these are the reader ids of different readers
    
    //making writer and reader threads for demonstration:

    pthread_create(&write_thread[0], NULL, writer, NULL);

    pthread_create(&write_thread[1], NULL, writer, NULL);
    
    for(int i = 0; i < 5; i++) {
        pthread_create(&read_thread[i], NULL,reader, (void *)&reader_id[i]);
    }
 
    pthread_create(&write_thread[2], NULL, writer, NULL);
    
    for(int i = 0; i < 5; i++) {
        pthread_create(&read_thread[i+5], NULL,reader, (void *)&reader_id[i+5]);
    }

    pthread_join(write_thread[0], NULL);
    pthread_join(write_thread[1], NULL);
    pthread_join(write_thread[2], NULL);

    for(int i = 0; i < 10; i++) {
        pthread_join(read_thread[i], NULL);
    }
    

  
    return 0;
    
}
