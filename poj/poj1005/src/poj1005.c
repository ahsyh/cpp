/*
 ============================================================================
 Name        : poj1005.c
 Author      : shi yihui
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

//#define _TEST_ 0
#define PI 3.14156265
int g_number;

typedef struct{
	double x;
	double y;
}point_t;

point_t* g_first = NULL;

int read_para()
{
	int i;
	//char buff[64];
#ifdef _TEST_
	FILE* fp=fopen("D:/workspace/poj1005/Debug/input.txt","r");
#endif

#ifdef _TEST_
#else
#endif

#ifdef _TEST_
	fscanf(fp,"%d\n",&g_number);
#else
	scanf("%d\n",&g_number);
#endif

	g_first = malloc(sizeof(point_t)*g_number);
	for(i=0;i<g_number;i++){
#ifdef _TEST_
		fscanf(fp,"%lf %lf\n",&g_first[i].x, &g_first[i].y);
#else
		scanf("%lf %lf\n",&g_first[i].x, &g_first[i].y);
#endif
	}

	return 0;
}

int calculate_year(int no){
	double d_year = PI*(g_first[no].x*g_first[no].x+g_first[no].y*g_first[no].y)/100.00;
	int    i_year = (int)d_year;

	if(d_year>i_year) return i_year+1;


	return i_year;
}

void output_result()
{
	int i;
	for(i=0;i<g_number;i++){
		printf("Property %d: This property will begin eroding in year %d.\n",i+1,calculate_year(i));
	}
	printf("END OF OUTPUT.\n");
	return;
}

int main(void) {
	read_para();
	output_result();

	return EXIT_SUCCESS;
}
