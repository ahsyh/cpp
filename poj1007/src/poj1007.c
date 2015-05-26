/*
 ============================================================================
 Name        : poj1007.c
 Author      : shi yihui
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define _TEST_ 0

char* g_dna;
int g_row;
int g_col;

int *g_weight;
int *g_order;

int cal_weight(char* dna,int length){
	int i=0;
	int j=0;

	int weight = 0;

	for(i=0;i<length;i++)
	{
		for(j=i+1;j<length;j++)
		{
			if(dna[j]<dna[i])
				weight ++;
		}

	}
	return weight;
}

void read_para(){
	int i;
	int max_weight;
#ifdef _TEST_
	FILE* fp=fopen("D:/workspace/poj1007/Debug/input.txt","r");
#else
#endif

#ifdef _TEST_
	fscanf(fp,"%d %d\n",&g_col,&g_row);
#else
	scanf("%d %d\n",&g_col,&g_row);
#endif

	g_dna=(char*)malloc(sizeof(char)*(g_row*(g_col+1)+1));
	g_weight=(int*)malloc(sizeof(int)*g_row);
	max_weight = g_col*(g_col-1)/2;

	for(i=0;i<g_row;i++)
	{
#ifdef _TEST_
		fscanf(fp,"%s\n",g_dna+i*(g_col+1));
#else
		scanf("%s\n",g_dna+i*(g_col+1));
#endif
		g_weight[i] = cal_weight(g_dna+i*(g_col+1),g_col);
		if(g_weight[i]>max_weight)
		{
			printf("EXCEPTION BIG WEIGHT FOUND, %d %s %d\n",
					i,g_dna+i*(g_col+1),g_weight[i]);
			exit(1);
		}
	}

	return;
}

void counter_sort(){
	char* tmp_buff;
	int i,j;
	int max_weight;

	tmp_buff = (char*)malloc(sizeof(char)*(g_row*(g_col+1)+1));
	memcpy(tmp_buff,g_dna,sizeof(char)*(g_row*(g_col+1)+1));

	max_weight = g_col*(g_col-1)/2;
	g_order = (int*)malloc(sizeof(int)*(1+max_weight));
	memset(g_order,0x00,sizeof(int)*(1+max_weight));

	for(i=0;i<g_row;i++)
	{
		g_order[g_weight[i]]++;
	}

	for(i=1;i<=max_weight;i++)
	{
		g_order[i]+=g_order[i-1];
	}

	for(i=g_row-1;i>=0;i--){
		j=g_order[g_weight[i]]-1;
		strcpy(g_dna+j*(g_col+1),tmp_buff+i*(g_col+1));
		g_order[g_weight[i]]--;
	}
	for(i=0;i<g_row;i++){
		printf("%s\n",g_dna+i*(g_col+1));
	}

	free(tmp_buff);
	return;
}

int main(void) {
	read_para();
	counter_sort();
	//puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
