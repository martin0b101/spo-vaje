#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define N_THR 2;

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;


double Ns = 0;
double Zs = 0;
int N = 1e9;

void* check_point_circle(void *args){
    double x, y;
    //int Z = 0;
    int N_times = 1e7;
    int break_this = 0;
    //pthread_mutex_lock(&mymutex);
    unsigned int seed = (unsigned int) args;
    
    //printf("hereOnTop\n");
    while(1){
        int i;
        pthread_mutex_lock(&mymutex);
        if (Ns >= N)
        {
            printf("Ns is %f\n", Ns);
            
            pthread_mutex_unlock(&mymutex);
            break;
        }
        pthread_mutex_unlock(&mymutex);
        
        int Z_times = 0;
        for (i = 0; i < N_times; i++)
        {   
            //printf("here\n");
            // get point (x, y)
            x = ((double) rand_r(&seed)) / RAND_MAX; 
            y = ((double) rand_r(&seed)) / RAND_MAX;  
            //printf("x: %f y: %f",x, y);
            // check if point is inside circle
            double result = (x*x) + (y*y);
            
            if (result <= 1)
            {
                Z_times++;  
            }
            /*if (Ns >= N)
            {
            //printf("Ns is %f\n", Ns);
                break_this = 1;
                break;
            }*/
            
        }
        
        pthread_mutex_lock(&mymutex);
        Zs+=Z_times;
        Ns+=N_times;
        pthread_mutex_unlock(&mymutex); 
        /*if (break_this)
        {
            break;
        }*/
    }
        
} 

int main(int argc, char *argv[]){

    
    struct timespec myTime;
    clock_gettime(CLOCK_REALTIME, &myTime); // program prevedi s stikalom -lrt
    
    int number_of_threads;
    if (argc > 1)
    {
        //printf("neki, %s",argv[1]);
        number_of_threads = atoi(argv[1]);
    }
    else{
        number_of_threads = 2;
    }
    printf("Number of threads: %d\n", number_of_threads);
    
    pthread_t id[number_of_threads];
    unsigned int i;
    int ret;
    
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for( i=0; i < number_of_threads; i++) {
        ret = pthread_create(&id[i], NULL, check_point_circle, (void *) i);
        if ( ret) {
            printf("Napaka (%d)\n", ret);
            exit(1);
        }
    }

    for (i = 0; i < number_of_threads; i++)
        pthread_join(id[i], NULL);
        
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken = ((double)(end.tv_sec-start.tv_sec)); // calculate the elapsed time

    
    pthread_mutex_lock(&mymutex);
    //calculate pi
    double pi = 4.0*Zs/Ns;
    pthread_mutex_unlock(&mymutex);
    
    printf("priblizek pi %f\n", pi);

    printf("Čas izvajanja: %f sekund \n", time_taken);

    pthread_mutex_destroy(&mymutex);

    pthread_exit(NULL); // zaradi tega se main ne konča, preden se ne zaključijo niti

    return 0;
}