#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define MAX 500
#define M 4
#define N 5

int multiply(int x, int res[], int res_size); 
void factorial(int n);

void main()
{
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
            factorial(*A[i][j]);
            printf("\t");
            sleep(2);
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

void factorial(int n) { 
	int res[MAX]; 
	res[0] = 1; 
	int res_size = 1; 

	for (int x=2; x<=n; x++) 
		res_size = multiply(x, res, res_size); 

	for (int i=res_size-1; i>=0; i--) 
        printf("%d",res[i]);
} 

int multiply(int x, int res[], int res_size) { 
	int carry = 0; 
	
	for (int i=0; i<res_size; i++) 
	{ 
		int prod = res[i] * x + carry; 
		res[i] = prod % 10; 
		carry = prod/10;	 
	} 

	while (carry) 
	{ 
		res[res_size] = carry%10; 
		carry = carry/10; 
		res_size++; 
	} 
	return res_size; 
} 
