/*
 ============================================================================
 Name        : poj1004.c
 Author      : shi yihui
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

//#define _TEST_

//int balance_list[12];

int convert_to_integer(char* buff)
{
	int i=0;
	int val=0;

	for(i=0;i<10;i++){
		if((buff[i]=='\0')||(buff[i]=='\n'))
			break;
		if(buff[i]=='.')
			continue;
		val = val*10 + buff[i]-'0';
	}

	return val;
}

int read_para()
{
	char buff[64];
#ifdef _TEST_
	FILE* fp=fopen("D:/workspace/poj1004/Debug/input.txt","r");
#endif

	int i;
	int accumulate=0;
	for(i=0;i<12;i++){
#ifdef _TEST_
		fgets(buff,32,fp);
#else
		gets(buff);
#endif
		accumulate+=convert_to_integer(buff);
	}
	return accumulate/12;
}

int main(void) {
	int val;

	val=read_para();

	printf("$%d.%02d\n",val/100,(val%100));

	return EXIT_SUCCESS;
}
