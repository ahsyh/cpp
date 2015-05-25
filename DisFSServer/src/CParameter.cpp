/*
 * CParameter.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: Shi Yihui
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "CParameter.h"

CParameter::CParameter():server_port(0) {
	memset(base_dir,0x00,128);
}

CParameter::~CParameter() {
}

int CParameter::read_para_file(){
	FILE* fp = fopen("./DisFSServer.ini","r");
	char  buff[256];
	char* head;
	char* content;
	int result = 0;

	if(fp==NULL){
		result = 1;
		return result;
	}

	while(fgets(buff,256,fp)!=NULL){
		int length = strlen(buff);
		if(buff[0]=='#'){
			continue;
		}
		while((buff[length-1]=='\n')){
			buff[length-1]='\0';
			length--;
		}
		head = strstr(buff,"=");
		if(head!=NULL)
			*head='\0';
		content=head+1;
		head=buff;

		if(strcmp(head,"server_port")==0){
			server_port = atoi(content);
			printf("read para, server port: %d\n",server_port);
		}
		else if(strcmp(head,"base_dir")==0){
			strcpy(base_dir,content);
			if(strcmp(base_dir,"")==0){
				strcpy(base_dir,".");
			}
			printf("read para, base dir:%s:\n",base_dir);
		}
		else{
			result = 2;
		}
	}

	fclose(fp);
	return result;
}
