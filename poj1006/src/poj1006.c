/*
 ============================================================================
 Name        : poj1006.c
 Author      : shi yihui
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

//#define _TEST_ 0

typedef struct para__t{
	int p;
	int e;
	int i;
	int n;
	struct para__t * next;
}para_t;

para_t* g_list;
para_t* g_end;

void read_para(){
	int p,e,i,n;
	para_t* tmp;
#ifdef _TEST_
	FILE* fp=fopen("D:/workspace/poj1006/Debug/input.txt","r");
#else
#endif

	g_list = g_end = NULL;
	while(1)
	{
#ifdef _TEST_
		fscanf(fp,"%d %d %d %d\n",&p,&e,&i,&n);
#else
		scanf("%d %d %d %d\n",&p,&e,&i,&n);
#endif

		if(p==-1)
			break;

		tmp = (para_t*)malloc(sizeof(para_t));
		tmp->p = p;
		tmp->e = e;
		tmp->i = i;
		tmp->n = n;

		tmp->next = NULL;
		if(g_end!=NULL)
			g_end->next = tmp;
		g_end = tmp;
		if(g_list==NULL)
			g_list = tmp;
	}

	return;
}

void output_result(){
	para_t* tmp;
	int d;

	tmp=g_list;
	d = 0;
	while(tmp!=NULL){
		d ++;
		printf("Case %d: the next triple peak occurs in %d days.\n",
				d,
				((5544*tmp->p+14421*tmp->e+1288*tmp->i+21251-tmp->n)%21252+1));
		tmp = tmp->next;
	}
	return;
}

int main(void) {
	read_para();
	output_result();
	return EXIT_SUCCESS;
}
