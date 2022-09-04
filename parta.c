#include<pthread.h>
#include <stdio.h>
#include<time.h>
#include <unistd.h>
#define MAX 1000
int buffer[MAX];

int count = 0;
int fill_ind=0;

pthread_cond_t wakecon, wakepro;
pthread_mutex_t mutex;
int consdone=1,prodone=0;

void writebuf(int value) {
	buffer[fill_ind] = value;
	fill_ind = (fill_ind + 1) % MAX;
	count++;
    printf("%d ",value);
}

void readbuf() {
    int i=0;
    printf("\nConsumer Thread: ");
    while(count>0){
        int tmp = buffer[i];
        i = (i + 1) % MAX;
        count--;
        printf("%d ",tmp);
        }
}

void flush() {
    for(int i=0;i<MAX;i++)
    {
        buffer[i]=-1;
    }

}


int timeelapsed(clock_t t){
    clock_t n;
    n = clock() - t;
    double time_taken = ((double)n)/CLOCKS_PER_SEC;
    //time taken is in seconds
    return time_taken*1000;//in milliseconds


}
void *producer(void *arg) {
    
    pthread_mutex_lock(&mutex);
    while(consdone==0){
        pthread_cond_wait(&wakepro,&mutex);
    }
    prodone=0;

    printf("Producer thread: ");
    fill_ind=0;
    int i=1;
    clock_t t;
    t = clock();
    while(timeelapsed(t)<1){
        //instead of 10 ms, we're doing 1 ms
        //its possible to change to 10 ms
        writebuf(i);
        i=(i+1)%MAX;
        sleep(0.1);
        
    }

    prodone=1;
    pthread_cond_signal(&wakecon);
    pthread_mutex_unlock(&mutex);

 }
 

void *consumer(void *arg) {
    pthread_mutex_lock(&mutex);
    while(prodone==0){
        pthread_cond_wait(&wakecon,&mutex);
    }
    consdone=0;

    readbuf();
    flush();

    consdone=1;
    pthread_cond_signal(&wakepro);
    pthread_mutex_unlock(&mutex);
    
}





int main(){
    pthread_t thread[2];
    pthread_create(&thread[0],NULL,producer,NULL);
    pthread_create(&thread[1],NULL,consumer,NULL);
    pthread_join(thread[0],NULL);
    pthread_join(thread[1],NULL);

    return 0;
}