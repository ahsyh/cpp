/*
 * telephone.c
 *
 *  Created on: 2013Äê11ÔÂ18ÈÕ
 *      Author: yihuishi
 */
#define _TEST_ 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _TEST_
#include <windows.h>
#include <winbase.h>
#endif


char letter_to_number['Z'];

typedef struct phone_counter__t{
	int phone_number;
	int counter;
	struct phone_counter__t* next;
	struct phone_counter__t* next_dup;
}phone_counter_t;

#define HAS_TABLE_SIZE 10000
phone_counter_t* g_first_ptr=(phone_counter_t*)1;
phone_counter_t* g_hash_table[HAS_TABLE_SIZE];

int g_number;

void init( ){
	char ch;
	int i;
	for(ch='A';ch<='Z';ch++){
		switch(ch){
		case 'A':
		case 'B':
		case 'C':
			letter_to_number[(int)ch]=(char)'2';
			break;
		case 'D':
		case 'E':
		case 'F':
			letter_to_number[(int)ch]=(char)'3';
			break;
		case 'G':
		case 'H':
		case 'I':
			letter_to_number[(int)ch]=(char)'4';
			break;
		case 'J':
		case 'K':
		case 'L':
			letter_to_number[(int)ch]=(char)'5';
			break;
		case 'M':
		case 'N':
		case 'O':
			letter_to_number[(int)ch]=(char)'6';
			break;
		case 'P':
		case 'R':
		case 'S':
			letter_to_number[(int)ch]=(char)'7';
			break;
		case 'T':
		case 'U':
		case 'V':
			letter_to_number[(int)ch]=(char)'8';
			break;
		case 'W':
		case 'X':
		case 'Y':
			letter_to_number[(int)ch]=(char)'9';
			break;
		case 'Q':
		case 'Z':
			letter_to_number[(int)ch]=(char)'-';
			break;
		default:
			break;
		}
	}

	for(i=0;i<HAS_TABLE_SIZE;i++){
		g_hash_table[i]=NULL;
	}
	return;
}



int convert_to_phonenumber(char* buff, int* phone){
	int l_phone;

	int i;
	for(i=0;i<strlen(buff);i++)
	{
		if((buff[i]>='a')&(buff[i]<='z'))
			buff[i]+=('A'-'a');

		if((buff[i]>='A')&(buff[i]<='Z'))
			buff[i]=(char)letter_to_number[(int)buff[i]];
	}

	int read, write;
	read=write=0;
	for(read=0;read<strlen(buff);read++)
	{
		if(buff[read]!='-')
		{
			if(read!=write)
				buff[write]=buff[read];
			write++;
		}
	}
	buff[write]='\0';
	sscanf(buff,"%d",&l_phone);

	*phone = l_phone;
	return 0;
}

phone_counter_t *get_counter_struct(char* buff, int phone){
	phone_counter_t* result;
	char hash_string[4];
	int hash_val=0;
	//int i;

	hash_string[0]='0'+(((buff[0]-'0')+(buff[2]-'0'))%10);
	hash_string[1]='0'+(((buff[1]-'0')+(buff[3]-'0'))%10);
	hash_string[2]='0'+(((buff[4]-'0')+(buff[6]-'0'))%10);
	hash_string[3]='0'+(((buff[5]-'0'))%10);
	hash_string[4]='\0';

	sscanf(hash_string,"%d",&hash_val);

	/*
	for(i=0;i<strlen(buff);i++){
		hash_val += buff[i];
	}
	hash_val = hash_val % HAS_TABLE_SIZE;
	*/

	result = g_hash_table[hash_val];

	while(result!=NULL)
	{
		if(result->phone_number==phone)
			break;
		else
			result = result->next;
	}

	if(result==NULL)
	{
		result = malloc(sizeof(phone_counter_t));
		result->phone_number = phone;
		result->counter = 0;
		result->next_dup = NULL;
		result->next = g_hash_table[hash_val];
		g_hash_table[hash_val] = result;
	}

	return result;
}

void read_para(){
	int i;
	int phone;
	phone_counter_t *phone_counter_struct;
	char buff[1024];
#ifdef _TEST_
	FILE* fp=fopen("D:/workspace/poj1002/Debug/input.txt","r");
#endif

#ifdef _TEST_
	fscanf(fp,"%d\n",&g_number);
#else
	scanf("%d\n",&g_number);
#endif

	for(i=0;i<g_number;i++){
#ifdef _TEST_
		fgets(buff,32,fp);
#else
		gets(buff);
#endif
		if(buff[strlen(buff)-1]=='\n')
			buff[strlen(buff)-1]='\0';

		convert_to_phonenumber(buff, &phone);

		phone_counter_struct = get_counter_struct(buff,phone);

		phone_counter_struct->counter ++;
		if((phone_counter_struct->counter>=2)&&(phone_counter_struct->next_dup==NULL))
		{
			phone_counter_struct->next_dup=g_first_ptr;
			g_first_ptr=phone_counter_struct;
		}
	}

	return;
}

typedef struct{
	char one_line[32];
}printout_t;

void swap(printout_t* list, int first, int second ){
	printout_t tmp;

	strcpy(tmp.one_line,list[first].one_line);
	strcpy(list[first].one_line,list[second].one_line);
	strcpy(list[second].one_line,tmp.one_line);
}

int partition_output(printout_t* list, int left, int right ){
	printout_t mid;
	int middle;

	strcpy(mid.one_line,list[left].one_line);
	int i=left+1;
	middle=i;
	for(;i<right;i++){
		if(strcmp(list[i].one_line,mid.one_line)<0){
			if(middle!=i)
			{
				swap(list,i,middle);
			}
			middle++;
		}
	}
	if((middle-1)!=(left))
		swap(list,left,middle-1);
	return middle-1;
}

void sort_output(printout_t* list, int left, int right ){
	int middle;
	if(left>=(right-1))return;
	if(left==(right-2))
	{
		if(strcmp(list[left].one_line,list[left+1].one_line)>0){
			swap(list,left,left+1);
		}
		return;
	}
	middle = partition_output(list,left,right);
	sort_output(list, left, middle);
	sort_output(list, middle+1, right);
}

void print_result(){
	phone_counter_t* curr=g_first_ptr;
	int counter = 0;
	int i;
	char *str;
	printout_t *output;

	if(g_first_ptr==(phone_counter_t*)1){
		printf("No duplicates. \n");
		return;
	}

	while(curr!=(phone_counter_t*)1)
	{
		counter ++;
		//printf("%07d %d\n",curr,g_counter[curr].counter);
		curr = curr->next_dup;
	}

	output = malloc(counter*sizeof(printout_t));

	curr = g_first_ptr;
	counter = 0;
	while(curr!=(phone_counter_t*)1)
	{
		str = output[counter].one_line;
		str += 1;
		sprintf(str,"%07d %d\n",curr->phone_number,curr->counter);
		output[counter].one_line[0]=output[counter].one_line[1];
		output[counter].one_line[1]=output[counter].one_line[2];
		output[counter].one_line[2]=output[counter].one_line[3];
		output[counter].one_line[3]='-';
		curr = curr->next_dup;
		counter ++;
	}

	sort_output(output,0,counter);

	for(i=0;i<counter;i++)
	{
		printf("%s",output[i].one_line);
	}
	return;
}

int main(){
#ifdef _TEST_
	DWORD dwStart, dwEnd1, dwEnd2, dwEnd3;
	dwStart = GetTickCount();
#endif
	init();
#ifdef _TEST_
	dwEnd1 = GetTickCount();
#endif
	read_para();
#ifdef _TEST_
	dwEnd2 = GetTickCount();
#endif
	print_result();
#ifdef _TEST_
	dwEnd3 = GetTickCount();
#endif

	printf("%d %d %d\n",(int)(dwEnd1-dwStart),(int)(dwEnd2-dwEnd1),(int)(dwEnd3-dwEnd2));
	return 0;
}
