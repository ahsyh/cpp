/*
 ============================================================================
 Name        : guass.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*
 * Original author:  UNKNOWN
 *
 * Modified:		 Shi Yihui(Oct 2013)
 */

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include <semaphore.h>


#define SWAP(a,b)       {double tmp; tmp = a; a = b; b = tmp;}

/* Solve the equation:
 *   matrix * X = R
 */

double **matrix, *X, *R;

#define MAX_TASK_NUM 8

typedef struct {
    int task_type;
    int pivot;
    int nsize;
    int begin;
    int end;
}task_data_t;

task_data_t task_list[MAX_TASK_NUM];
int         id[MAX_TASK_NUM];
pthread_t   tid[MAX_TASK_NUM];
sem_t       g_work_sem[MAX_TASK_NUM];

/* Pre-set solution. */
double *X__;

/* Initialize the matirx. */

void initMatrix(const char *fname, int *size)
{
    FILE *file;
    int l1, l2, l3;
    double d;
    int i, j;
    double *tmp;
    char buffer[1024];
    int nsize;

    if ((file = fopen(fname, "r")) == NULL) {
        fprintf(stderr, "The matrix file open error\n");
        exit(-1);
    }

    /* Parse the first line to get the matrix size. */
    fgets(buffer, 1024, file);
    sscanf(buffer, "%d %d %d", &l1, &l2, &l3);
    nsize = l1;
    *size = nsize;
#ifdef DEBUG
    fprintf(stdout, "matrix size is %d\n", nsize);
#endif

    /* Initialize the space and set all elements to zero. */
    matrix = (double**)malloc(nsize*sizeof(double*));
    assert(matrix != NULL);
    tmp = (double*)malloc(nsize*nsize*sizeof(double));
    assert(tmp != NULL);
    for (i = 0; i < nsize; i++) {
        matrix[i] = tmp;
        tmp = tmp + nsize;
    }
    for (i = 0; i < nsize; i++) {
        for (j = 0; j < nsize; j++) {
            matrix[i][j] = 0.0;
        }
    }

    /* Parse the rest of the input file to fill the matrix. */
    for (;;) {
        fgets(buffer, 1024, file);
        sscanf(buffer, "%d %d %lf", &l1, &l2, &d);
        if (l1 == 0) break;

        matrix[l1-1][l2-1] = d;
#ifdef DEBUG
        fprintf(stdout, "row %d column %d of matrix is %e\n", l1-1, l2-1, matrix[l1-1][l2-1]);
#endif
    }

    fclose(file);
    return ;
}

/* Initialize the right-hand-side following the pre-set solution. */

void initRHS(int nsize)
{
    int i, j;

    X__ = (double*)malloc(nsize * sizeof(double));
    assert(X__ != NULL);
    for (i = 0; i < nsize; i++) {
        X__[i] = i+1;
    }

    R = (double*)malloc(nsize * sizeof(double));
    assert(R != NULL);
    for (i = 0; i < nsize; i++) {
        R[i] = 0.0;
        for (j = 0; j < nsize; j++) {
            R[i] += matrix[i][j] * X__[j];
        }
    }
}

/* Initialize the results. */

void initResult(int nsize)
{
    int i;

    X = (double*)malloc(nsize * sizeof(double));
    assert(X != NULL);
    for (i = 0; i < nsize; i++) {
        X[i] = 0.0;
    }
}

/* Get the pivot - the element on column with largest absolute value. */

void getPivot(int nsize, int currow)
{
    int i, pivotrow;

    pivotrow = currow;
    for (i = currow+1; i < nsize; i++) {
        if (fabs(matrix[i][currow]) > fabs(matrix[pivotrow][currow])) {
            pivotrow = i;
        }
    }

    if (fabs(matrix[pivotrow][currow]) == 0.0) {
        fprintf(stderr, "The matrix is singular\n");
        exit(-1);
    }

    if (pivotrow != currow) {
#ifdef DEBUG
        fprintf(stdout, "pivot row at step %5d is %5d\n", currow, pivotrow);
#endif
        for (i = currow; i < nsize; i++) {
            SWAP(matrix[pivotrow][i],matrix[currow][i]);
        }
        SWAP(R[pivotrow],R[currow]);
    }
}

/* For all the rows, get the pivot and eliminate all rows and columns
 * for that particular pivot row. */
void *
work_thread (void *lp);

void computeGauss(int nsize, int work_thread_num)
{
    int i, j, k;
    double pivotval;
    int    unfinished_task_num = 0;
    struct timeval start, finish;
    gettimeofday(&start, 0);

    for (i = 0; i < nsize; i++) {
        getPivot(nsize,i);

        /* Scale the main row. */
        pivotval = matrix[i][i];
        if (pivotval != 1.0) {
            matrix[i][i] = 1.0;
            for (j = i + 1; j < nsize; j++) {
                matrix[i][j] /= pivotval;
            }
            R[i] /= pivotval;
        }

        ///* Factorize the rest of the matrix. */
        //memset(task_list, 0x00, sizeof(task_data_t)*MAX_TASK_NUM);
        for (k = 0; k < work_thread_num; k++) {
            task_list[k].task_type = 0;
        }
        int step = (nsize-i-1)/work_thread_num+1;
        //step = (step<5)?5:step;
        int curr = i+1;
        for (k = 0; k < work_thread_num; k++) {
            id[k] = k;
            task_list[k].task_type = 1;
            task_list[k].pivot = i;
            task_list[k].nsize = nsize;
            task_list[k].begin = curr;
            curr += step;
            if(curr>=nsize)
            {
                task_list[k].end = nsize;
                break;
            }
            else
            {
                task_list[k].end = curr;
            }
        }

        unfinished_task_num = 0;
        for (k = 0; k < work_thread_num; k++)
        {
            unfinished_task_num++;
            sem_post(g_work_sem+k);
        }

        work_thread(&id[0]);
        // wait for all threads to finish

        for(;;)
        {
            sem_wait(g_work_sem);

            unfinished_task_num--;
            if(unfinished_task_num==0)
                break;
        }
    }
    gettimeofday(&finish, 0);
    fprintf(stdout, "Time:  %f seconds\n", (finish.tv_sec - start.tv_sec) + (finish.tv_usec - start.tv_usec)*0.000001);
}

void setCpuset(int id)
{
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(id, &cpuset);
    if( sched_setaffinity( 0, sizeof(cpuset), &cpuset ) != 0 )
    {
        fprintf(stdout, "Pthread set cpu set failed\n");
    }
}

void *
work_thread (void *lp)
{
    int i,j,k,begin,end;
    int task_id = *((int *) lp);
    double pivotval;
    int nsize;

    if(task_id!=0)
    {
        setCpuset(task_id);
    }

    for(;;)
    {

        sem_wait(g_work_sem+task_id);

        if(task_list[task_id].task_type==1)
        {
            i       = task_list[task_id].pivot;
            begin   = task_list[task_id].begin;
            end     = task_list[task_id].end;
            nsize   = task_list[task_id].nsize;

            /* Factorize the rest of the matrix. */
            for (j = begin; j < end; j++) {
                pivotval = matrix[j][i];
                matrix[j][i] = 0.0;
                for (k = i + 1; k < nsize; k++) {
                    matrix[j][k] -= pivotval * matrix[i][k];
                }
                R[j] -= pivotval * R[i];
            }
        }
        else if(task_list[task_id].task_type==2)
        {
            i       = task_list[task_id].pivot;
            begin   = task_list[task_id].begin;
            end     = task_list[task_id].end;
            nsize   = task_list[task_id].nsize;

            for(j=begin; j<end; j++)
            {
                R[j] -= matrix[j][i] * R[i];
            }
        }

        sem_post(g_work_sem);

        if(task_id==0)
        {
            return NULL;
        }
    }

    return NULL;
}

/* Solve the equation. */

void solveGauss(int nsize, int work_thread_num)
{
    int i, k;
    int    unfinished_task_num = 0;

    /*
    X[nsize-1] = R[nsize-1];
    for (i = nsize - 2; i >= 0; i --) {
        X[i] = R[i];
        for (j = nsize - 1; j > i; j--) {
            X[i] -= matrix[i][j] * X[j];
        }
    }
    */

    for (i = nsize - 1; i >= 1; i --)
    {
        X[i] = R[i];

        ///* Factorize the rest of the matrix. */
        //memset(task_list, 0x00, sizeof(task_data_t)*MAX_TASK_NUM);
        for (k = 0; k < work_thread_num; k++) {
            task_list[k].task_type = 0;
        }

        int step = i/work_thread_num+1;
        //step = (step<5)?5:step;
        int curr = 0;
        for (k = 0; k < work_thread_num; k++) {
            id[k] = k;
            task_list[k].task_type = 2;
            task_list[k].pivot = i;
            task_list[k].nsize = nsize;
            task_list[k].begin = curr;
            curr += step;
            if(curr>=i)
            {
                task_list[k].end = i;
                //fprintf(stdout, "task%d, begin:%d, end:%d\n",k,task_list[k].begin,task_list[k].end);
                break;
            }
            else
            {
                task_list[k].end = curr;
                //fprintf(stdout, "task%d, begin:%d, end:%d\n",k,task_list[k].begin,task_list[k].end);
            }
        }

        unfinished_task_num = 0;
        for (k = 0; k < work_thread_num; k++)
        {
            unfinished_task_num++;
            sem_post(g_work_sem+k);
        }

        work_thread(&id[0]);
        // wait for all threads to finish

        for(;;)
        {
            sem_wait(g_work_sem);

            unfinished_task_num--;
            if(unfinished_task_num==0)
                break;
        }

    }
    X[0] = R[0];


#ifdef DEBUG
    fprintf(stdout, "X = [");
    for (i = 0; i < nsize; i++) {
        fprintf(stdout, "%.6f ", X[i]);
    }
    fprintf(stdout, "];\n");
#endif
}

void initThread(int work_thread_num)
{
    int i;
    for(i = 0; i < work_thread_num; i++)
    {
        sem_init(g_work_sem+i, 0, 0);
    }

    pthread_attr_t attr;
    pthread_attr_init (&attr);
    pthread_attr_setscope (&attr, PTHREAD_SCOPE_SYSTEM);

    id[0]=0;
    tid[0] = pthread_self();
    for (i = 1; i < work_thread_num; i++)
    {
        id[i] = i;
        pthread_create (&tid[i], &attr, work_thread, &id[i]);
    }

    //set CPU bind relationship for all threads
    setCpuset(0);
}


void cleanupThread(int work_thread_num)
{
    int i;
    for (i = 1; i < work_thread_num; i++)
    {
        pthread_cancel (tid[i]);
    }

    for (i = 1; i < work_thread_num; i++)
    {
        pthread_join (tid[i],NULL);
    }

    for(i = 0; i < work_thread_num; i++)
    {
        sem_destroy(g_work_sem+i);
    }
}

void verifyResult(int nsize)
{
    double  error;
    int i;
    error = 0.0;
    for (i = 0; i < nsize; i++) {
        double error__ = (X__[i]==0.0) ? 1.0 : fabs((X[i]-X__[i])/X__[i]);
        if (error < error__) {
            error = error__;
        }
    }
    fprintf(stdout, "Error: %e\n", error);
}

int main(int argc, char *argv[])
{
    int     c;
    char*   filename;
    int     l_nsize = 0;
    int     l_thread_num = 1;

    while ((c = getopt (argc, argv, "f:p:")) != -1)
    {
        switch (c) {
        case 'f':
            filename = optarg;
            break;
        case 'p':
            l_thread_num = atoi (optarg);
            break;
        default:
            break;
        }
    }

    initThread(l_thread_num);

    initMatrix(filename, &l_nsize);
    initRHS(l_nsize);
    initResult(l_nsize);
    computeGauss(l_nsize, l_thread_num);
    solveGauss(l_nsize, l_thread_num);
    verifyResult(l_nsize);

    cleanupThread(l_thread_num);

    return 0;
}
