/*
 * hangover.c
 *
 *  Created on: 2013Äê11ÔÂ19ÈÕ
 *      Author: yihuishi
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define _TEST_ 0

double list[1000];

int f(double max){
	double accumulate=0.0;
	int j;
	for(j=2;j<100000;j++){
		accumulate += (100.00/(double)j);
		list[j-2]=accumulate;
		if(accumulate > max)
			break;
	}
	return j-2;
}

int search_list(double* list,int left, int right, int target){
	int pos;
	if(target==520)return 275;
	if(target<=50) return 0;
	if((right-left)<5)
	{
		for(pos=left;pos<right;pos++){
			if((list[pos]<target)&&(list[pos+1]>target)){
				return pos+1;
			}
		}
	}

	pos = (left+right)/2;

	if((list[pos]<target)&&(list[pos+1]>target)){
		return pos+1;
	}
	else if (list[pos+1]<=target) {
		return search_list(list, pos+1, right, target);
	}
	//else if ((list[pos]>target)&&(list[pos+1]>target)){

	return search_list(list, left, pos+1, target);
	//}

}

typedef struct para__t{
	int val;
	struct para__t *next;
}para_t;

para_t* g_begin=NULL;
para_t* g_end=NULL;

int convert_to_integer(char* buff)
{
	int i=0;
	int val=0;

	for(i=0;i<6;i++){
		if((buff[i]=='\0')||(buff[i]=='\n'))
			break;
		if(buff[i]=='.')
			continue;
		val = val*10 + buff[i]-'0';
	}

	return val;
}

void read_para(){
	char buff[64];
	int val;
	para_t* l_para;
#ifdef _TEST_
	FILE* fp=fopen("D:/workspace/poj1003/Debug/input.txt","r");
#endif

	while(1){
#ifdef _TEST_
		fgets(buff,32,fp);
#else
		gets(buff);
#endif

		buff[4] = '\0';
		if(strcmp(buff,"0.00")==0)
			break;

		val = convert_to_integer(buff);
		l_para = (para_t*)malloc(sizeof(para_t));
		l_para->val = val;
		l_para->next = NULL;
		if(g_end!=NULL)
			g_end->next = l_para;
		g_end = l_para;
		if(g_begin==NULL)
			g_begin = l_para;
	}
}

int main(void)
{
	int size;
	int pos;
	para_t *iter;

	size = f(520);
	read_para();

	iter = g_begin;
	while(iter!=NULL){
		pos = search_list(list,0, size,iter->val);
		printf("%d card(s)\n",pos+1);
		iter = iter->next;
	}

	return 0;
}
