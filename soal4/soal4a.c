#include <stdio.h>
#include <stdlib.h>
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

void *count(void *ptr) /* thread for every element in matrix */
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

int main(int argc, char **argv)
{
    int i, j;
    int thread_counter = 0;
    
    pthread_t thread[NUM_THREADS];
    
    /* create M * N worker threads */
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++) 
        {
            struct v *data = (struct v *) malloc(sizeof(struct v));
            data->i = i;
            data->j = j;
            // printf("%d %d\n",data->i,data->j);
            /* create the thread passing it data as a paramater*/
            pthread_create(&thread[thread_counter], NULL, count, data);
            pthread_join(thread[thread_counter], NULL);
            thread_counter++;
        }
    }
    
    /* Waiting for threads to complete */
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }
    
    for(i = 0; i < M; i++)
    { 
        for(j = 0; j < N; j++)
        { 
            printf("%d", C[i][j]);
            if(j!=N-1)
                printf("\t");
        }
        printf("\n");
    }
    return 0;
}


