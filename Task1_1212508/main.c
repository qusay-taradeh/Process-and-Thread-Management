#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

#define N 100                            // number of rows and cols
#define THREADS_NUM 2                    // number of threads
#define PROCESSES_NUM 2                  // number of processes
int idMatrix[N][N], idXyearMatrix[N][N]; // idMatrix: matrix of my ID number, idXyearMatrix: matrix of my ID multiplied by my birth year
int resultMatrix[N][N];                  // matrix wich is the result of multiplication between ID matrix and ID * year matrix

int childProcess();
void *threadFunc(void *arg);
void mulMatrix();
void printMatrix(int matrix[N][N]);
void menu();

int main(int argc, char *argv[])
{
    struct timeval start, end;                        // to measure the time.
    float tm;                                         // time measured
    int i, j, k = 0, l = 0;                           // variables for loops
    int id[7] = {1, 2, 1, 2, 5, 0, 8};                // my ID represented as digits in array
    int idXyear[10] = {2, 4, 2, 8, 6, 5, 3, 5, 2, 4}; // idXyear = id * year = 1212508 * 2003 = 2428653524
    pthread_t p[THREADS_NUM];                         // array of threads to be use
    pthread_attr_t detached;

    ////////initializing two matrices to do the task on them//////////////
    for (i = 0; i < N; i++) // loop to fill ID matrix and ID * Year matrix
        for (j = 0; j < N; j++)
        {
            if (k > 6)
                k = 0;
            if (l > 9)
                l = 0;
            idMatrix[i][j] = id[k];
            idXyearMatrix[i][j] = idXyear[l];
            k++;
            l++;
        }
    //////////////end of initializing/////////
    int option;
    menu();
    scanf("%d", &option);
    if (option == 1)
    {
        childProcess();
    }
    else if (option == 2) // Joinable thrads
    {
        gettimeofday(&start, NULL);
        int m;
        for (m = 0; m < THREADS_NUM; m++) // creating threads loop
        {
            int *a = malloc(sizeof(int));
            *a = m * N / THREADS_NUM;
            if (pthread_create(&p[m], NULL, &threadFunc, a) != 0)
            {
                perror("Failed to create thread");
            }
        }
        for (m = 0; m < THREADS_NUM; m++) // joining threads loop
            if (pthread_join(p[m], NULL) != 0)
                perror("Failed to join thread");

        gettimeofday(&end, NULL);
        tm = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
        printf("Time of use %d joinable threads is: %fs\n", THREADS_NUM, tm);
    }
    else if (option == 3) // Detached threads
    {
        pthread_attr_init(&detached);
        pthread_attr_setdetachstate(&detached, PTHREAD_CREATE_DETACHED);
        gettimeofday(&start, NULL);
        int m;
        for (m = 0; m < THREADS_NUM; m++) // creating threads loop
        {
            int *a = malloc(sizeof(int));
            *a = m * N / THREADS_NUM;
            if (pthread_create(&p[m], &detached, &threadFunc, a) != 0)
            {
                perror("Failed to create thread");
            }
            pthread_detach(p[m]); // detaching threads
        }
        gettimeofday(&end, NULL);
        tm = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
        printf("Time of use %d detached threads is: %fs\n", THREADS_NUM, tm);
        pthread_attr_destroy(&detached);
        pthread_exit(0);
    }
    else if (option == 4)
    {
        printf("\nThx, Bye!\n");
        return 0;
    }
    else
        printf("\nRerun the program and enter valid option please!\n");

    return 0;
}

int childProcess() // child processes
{
    struct timeval start, end;    // variables of time to be measured
    float tm;                     // tm: time calculated
    pid_t pid[PROCESSES_NUM];     // pid : process id
    int pipeFD[PROCESSES_NUM][2]; // pipeFD : pipe file discovery for IPC , 0 for read and 1 for write
    int i, j;

    for (i = 0; i < PROCESSES_NUM; i++)
        if (pipe(pipeFD[i]) == -1) // error handling when creating pipe failure and this return 1
        {
            printf("Can't create pipe!\n");
            return 1;
        }

    gettimeofday(&start, NULL);

    for (i = 0; i < PROCESSES_NUM; i++) // lopp initialize child processes
    {
        pid[i] = fork();  // forking the process
        if (pid[i] == -1) // error handling when fork failure and this return 2
        {
            printf("An error occured with fork!\n");
            return 2;
        }

        if (pid[i] == 0) // Child process
        {
            close(pipeFD[i][0]); // close reading of current process
            for (j = 0; j < PROCESSES_NUM; j++)
            {
                if (j != i)
                {
                    close(pipeFD[j][0]); // close read of other processes
                    close(pipeFD[j][1]); // close write of other processes
                }
            }

            mulMatrix();

            if (write(pipeFD[i][1], resultMatrix, sizeof(resultMatrix)) == -1) // writing and handling error
                return 3;

            close(pipeFD[i][1]); // close writing in process
            return 0;
        }
    }

    // Parent process
    for (i = 0; i < PROCESSES_NUM; i++) // close write of each process
        close(pipeFD[i][1]);

    gettimeofday(&end, NULL);

    for (i = 0; i < PROCESSES_NUM; i++)
        waitpid(pid[i], NULL, 0); // waiting for each process

    for (i = 0; i < PROCESSES_NUM; i++)
        if (read(pipeFD[i][0], resultMatrix + i * N / PROCESSES_NUM, sizeof(resultMatrix) / PROCESSES_NUM) == -1) // reading from each process
            return 4;

    for (i = 0; i < PROCESSES_NUM; i++) // close read of each process
        close(pipeFD[i][0]);

    tm = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    printf("Time of use %d child processes is: %fs\n", PROCESSES_NUM, tm);
    return 0;
}

void *threadFunc(void *arg) // function of any type of threads that do multiplication also
{
    int index = *(int *)arg; // index as a limit for each thread to do part of multiplication
    int i, j, k;
    for (i = 0; i < N / THREADS_NUM; i++)
        for (j = 0; j < N; j++)
        {
            resultMatrix[i + index][j] = 0;
            for (k = 0; k < N; k++)
                resultMatrix[i + index][j] += (idMatrix[i + index][k] * idXyearMatrix[k][j]);
        }
    free(arg);
}

void mulMatrix() // multiplication code
{
    int i, j, k;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
        {
            resultMatrix[i][j] = 0;
            for (k = 0; k < N; k++)
                resultMatrix[i][j] += (idMatrix[i][k] * idXyearMatrix[k][j]);
        }
}

void printMatrix(int matrix[N][N]) // for printing the result or any passed matrix
{
    int i, j;
    printf("Result:\n");
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            printf("%d ", matrix[i][j]);
    printf("\n");
}

void menu() // simple menu to choose the type of execution
{
    printf("\n===========Menu==========\n");
    printf("1. Do with child process.\n");
    printf("2. Do with joinable threads.\n");
    printf("3. Do with detached threads.\n");
    printf("4. Exit.\n");
    printf("Enter number of option: ");
}
