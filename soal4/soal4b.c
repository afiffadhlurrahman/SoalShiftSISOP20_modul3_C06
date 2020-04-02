#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>

#define M 4
#define N 5
#define NUM_THREADS M * N

int hasil[M][N];
int print[M][N];

struct v
{
    int i; /* row */
    int j; /* column */
};

void *runner1(void *ptr);

void main()
{
    int i, j;
    int thread_counter = 0;
    pthread_t workers[NUM_THREADS];

    key_t key = 1234;
    int *A[M][N];
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            A[i][j] = shmat(shmid, NULL, 0);
        }
    }

    for(int i = 0;i<M;i++){
        for(int j=0;j<N;j++){
            hasil[i][j]=*A[i][j];
            sleep(2);
        }
    }

    for(int i = 0;i<M;i++){
        for(int j=0;j<N;j++){
            struct v *data = (struct v *) malloc(sizeof(struct v));
            data->i = i;
            data->j = j;
            /* create the thread passing it data as a paramater*/
            pthread_create(&workers[thread_counter], NULL, runner1, data);
            pthread_join(workers[thread_counter], NULL);
            thread_counter++;
        }
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(workers[i], NULL);
    }

    for(int i = 0;i < M;i++){
        for(int j = 0;j < N;j++){
            printf("%d\t",hasil[i][j]);
        }
        printf("\n");
    }

    for(int i = 0;i < M;i++){
        for(int j = 0;j < N;j++){
            shmdt(A[i][j]);   
        }
    } 
    shmctl(shmid, IPC_RMID, NULL);
}

void *runner1(void *ptr)
{    
    /* Casting paramater to struct v pointer */
    struct v *data = ptr;
    int i, sum = 0;
    
    for(i = 1; i <= hasil[data->i][data->j]; i++)
    {    
        sum += i;
    }
    
    hasil[data->i][data->j] = sum;
    pthread_exit(0);
}
