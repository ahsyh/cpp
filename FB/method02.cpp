/*
 * method02.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: yihuishi
 */

//============================================================================
// Name        : FB_002.cpp
// Author      : syh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef _TEST_
static FILE* gFile;
#else
#endif

typedef struct{
    int * guess;
    int score;
} one_guess_t;

typedef struct{
    int n;
    int k;
    int q;
    one_guess_t *list;
} record_t;

record_t* g_record;
int g_record_num;

int read_one_record(int i)
{
    int j,k;
#ifdef _TEST_
    fscanf(gFile,"%d %d %d\n",&g_record[i].n,&g_record[i].k,&g_record[i].q);
#else
    scanf("%d %d %d\n",&g_record[i].n,&g_record[i].k,&g_record[i].q);
#endif
    g_record[i].list = (one_guess_t*)malloc(sizeof(one_guess_t)*g_record[i].q);
    for(j=0;j<g_record[i].q;j++)
    {
        g_record[i].list[j].guess = (int*)malloc(sizeof(int)*g_record[i].k);
        for(k=0;k<g_record[i].k;k++)
        {
#ifdef _TEST_
            fscanf(gFile,"%d",&g_record[i].list[j].guess[k]);
#else
            scanf("%d",&g_record[i].list[j].guess[k]);
#endif
        }
#ifdef _TEST_
        fscanf(gFile,"%d\n",&g_record[i].list[j].score);
#else
        scanf("%d\n",&g_record[i].list[j].score);
#endif
    }
    return 0;
}

int read_para()
{
    int i;

#ifdef _TEST_
    fscanf(gFile,"%d",&g_record_num);
#else
    scanf("%d",&g_record_num);
#endif

    g_record = (record_t*)malloc(sizeof(record_t)*g_record_num);
#ifdef _TEST_
    fscanf(gFile,"\n");
#else
    scanf("\n");
#endif

    for(i=0;i<g_record_num;i++)
    {
        read_one_record(i);
    }
    return 0;
}

bool check_one_guess(int n, int* result, int* guess, int score)
{
    int i;
    int local_score=0;

    for(i=0;i<n;i++)
    {
        if(result[i]==guess[i])
        {
            local_score ++;
        }
        if(local_score>score)
            return false;
    }

    if(local_score==score)
    {
        return true;
    }

    return false;
}

bool check_result(int n, int* result, one_guess_t* list, int q)
{
    int i;

    for(i=0;i<q;i++)
    {
        if(!check_one_guess(n, result, list[i].guess, list[i].score))
        {
            return false;
        }
    }
#ifdef _TEST_
    printf("find one result\n");
    for(i=0;i<n;i++)
    {
        printf("%d ",result[i]);
    }
    printf("\n");
#else
#endif
    return true;
}

bool g_find_match;

void generate_all_poss_and_check(int n, int k, int q, int curr_k, int* result, one_guess_t* guess_list)
{
    int i;

    if(g_find_match==true)
        return;

    if(curr_k>=k)
    {
        if(check_result(k,result,guess_list,q))
        {
            g_find_match = true;
        }
        return;
    }

    for(i=0;i<n;i++)
    {
        result[curr_k]=i;
        generate_all_poss_and_check(n,k,q,curr_k+1,result,guess_list);
    }

    return;
}

int check_one_case(int i)
{
    bool result=false;
    int n,k,q;
    one_guess_t* list = g_record[i].list;
    n = g_record[i].n;
    k = g_record[i].k;
    q = g_record[i].q;

    int* poss_result = (int*)malloc(sizeof(int)*k);

    g_find_match = false;
    generate_all_poss_and_check(n,k,q,0,poss_result,list);
    if(g_find_match)
    {
        result = true;
    }

    delete(poss_result);

    return result;
}

int verify_data()
{
    int i;

    for(i=0;i<g_record_num;i++)
    {
        if(check_one_case(i))
        {
            printf("Yes\n");
        }
        else
        {
            printf("No\n");
        }
    }
    return 0;
}

int method02()
{
#ifdef _TEST_
    gFile = fopen("input001.txt","r");
#else
#endif
    read_para();
    verify_data();
    return 0;
}


