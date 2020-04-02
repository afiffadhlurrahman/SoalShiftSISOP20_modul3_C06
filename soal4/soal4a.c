#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

#define M 4
#define K 2
#define N 5
#define NUM_THREADS M * N

/* Global variables for threads to share */
int A[M][K] = {{1, 4}, {2, 5}, {3, 5}, {2,3}};
int B[K][N] = {{8, 7, 6, 1, 2}, {5, 4, 3, 2, 1}};
int C[M][N];

/* Structure for passing data to threads */
struct v
{
    int i; /* row */
    int j; /* column */
};

void *runner(void *ptr); /* the thread */

void main()
{   
    int i, j;
    int thread_counter = 0;
    
    pthread_t workers[NUM_THREADS];
    
    /* create M * N worker threads */
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++) 
        {
            struct v *data = (struct v *) malloc(sizeof(struct v));
            data->i = i;
            data->j = j;
            /* create the thread passing it data as a paramater*/
            pthread_create(&workers[thread_counter], NULL, runner, data);
            pthread_join(workers[thread_counter], NULL);
            thread_counter++;
        }
    }
    
    /* Waiting for threads to complete */
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(workers[i], NULL);
    }

    //shared memory
    // shared memory
    key_t key = 1234;
    int *share[M][N];
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);

    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            share[i][j] = shmat(shmid, NULL, 0);
        }
    }

    for(i = 0; i < M; i++)
    { 
        for(j = 0; j < N; j++)
        { 
            *share[i][j] = C[i][j];
            printf("%d\t", *share[i][j]);\
            sleep(2);
        }
        printf("\n");
    }

    for(int i = 0;i < M;i++){
        for(int j = 0;j < N;j++){
            shmdt(share[i][j]);
        }
    }
    shmctl(shmid, IPC_RMID, NULL);  
}

void *runner(void *ptr)
{    
    /* Casting paramater to struct v pointer */
    struct v *data = ptr;
    int i, sum = 0;
    
    for(i = 0; i < K; i++)
    {    
        sum += A[data->i][i] * B[i][data->j];
    }
    
    C[data->i][data->j] = sum;
    pthread_exit(0);
}
