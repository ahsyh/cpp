/*
 * task_archive.cpp
 *
 *  Created on: Nov 24, 2013
 *      Author: Shi Yihui
 */

#include <stdio.h>
#include <string.h>
#include "CTasks.h"
#include "CSeqGenerator.h"

extern CTransactions g_transactions;
extern CSeqGenerator* g_seq_gen;

const char ARCHIVE_FILE_NAME[]="./.uncommit_transactions.ini";

typedef struct {
	enum method_e method;
	int transaction_id;
	int seq;
	int content_length;
	char content[0];
}one_task_t;

typedef struct {
	int transaction_id;
	int max_seq;
	int seq_number;
	bool is_commited;
	int	file_length;
	char filename[0];
}one_transaction_t;

void read_transaction()
{
	FILE* fp=fopen(ARCHIVE_FILE_NAME,"r");

	if(fp!=NULL)
	{
		char buff[1024];
		int buff_size;
		int trans_num;
		int max_transid=-1;
		one_transaction_t* one_transaction;
		one_task_t* one_task;
		COneTransaction *one_c_trans;

		fread(buff, sizeof(int), 1, fp);
		trans_num = *((int*)buff);
		printf("read trans number: %d\n", trans_num);

		for(int j=0;j<trans_num;j++){
			fread(buff, sizeof(int), 1, fp);
			buff_size = *((int*)buff);
			fread(buff, buff_size, 1, fp);
			one_transaction = (one_transaction_t*)buff;

			g_transactions.add_one_transaction(one_transaction->transaction_id);
			one_c_trans = g_transactions.get_one_transaction(one_transaction->transaction_id);
			one_c_trans->seq_list.reset();
			one_c_trans->max_seq = one_transaction->max_seq;
			one_c_trans->is_commited = one_transaction->is_commited;
			memcpy(one_c_trans->file_name,one_transaction->filename,one_transaction->file_length);
			if(one_transaction->transaction_id>max_transid)
			{
				max_transid = one_transaction->transaction_id;
			}
			printf("read trans %d\n", one_transaction->transaction_id);

			int seq_number=one_transaction->seq_number;
			for(int i=0;i<seq_number;i++){
				fread(buff, sizeof(int), 1, fp);
				buff_size = *((int*)buff);
				fread(buff, buff_size, 1, fp);
				one_task = (one_task_t*)buff;
				printf("read task id %d seq %d\n", one_task->transaction_id, one_task->seq);

				one_c_trans->add_one_task(
						one_task->method,
						one_task->transaction_id,
						one_task->seq,
						one_task->content_length,
						one_task->content);
			}
		}
		fclose(fp);
		if(max_transid!=-1){
			max_transid++;
			g_seq_gen->m_curr_seq = max_transid;
		}
	}


	fp=fopen(ARCHIVE_FILE_NAME,"w");
	int trans_num = 0;
	fwrite(&trans_num,sizeof(int),1,fp);
	fclose(fp);
	return;
}

void write_transaction()
{
	FILE* fp=fopen(ARCHIVE_FILE_NAME,"w");

	{
		char buff[1024];
		one_transaction_t *one_transaction;
		one_task_t *one_task;

		int trans_num = g_transactions.all_transactions.size();
		if(trans_num<=0)
		{
			trans_num = 0;
			fwrite(&trans_num,sizeof(int),1,fp);
			fclose(fp);
			return;
		}
		fwrite(&trans_num,sizeof(int),1,fp);

		vector<COneTransaction>::iterator iter1;
		for(iter1=g_transactions.all_transactions.begin();
				iter1!=g_transactions.all_transactions.end();
				iter1++)
		{
			one_transaction = (one_transaction_t*)buff;
			one_transaction->transaction_id = (*iter1).transaction_id;
			one_transaction->max_seq = (*iter1).max_seq;
			one_transaction->seq_number = (*iter1).seq_list.count();
			one_transaction->is_commited = (*iter1).is_commited;
			one_transaction->file_length = strlen((*iter1).file_name);
			memcpy(one_transaction->filename, (*iter1).file_name, one_transaction->file_length);

			int trans_size = sizeof(one_transaction_t)+one_transaction->file_length;
			fwrite(&trans_size, sizeof(int), 1, fp);
			fwrite(buff, trans_size, 1, fp);
			printf("write trans %d\n", one_transaction->transaction_id);

			vector<COneTask>::iterator iter2;
			for(iter2=(*iter1).all_tasks.begin();
					iter2!=(*iter1).all_tasks.end();
					iter2++)
			{
				int task_size;

				one_task = (one_task_t*)buff;
				one_task->transaction_id = (*iter2).transaction_id;
				one_task->seq = (*iter2).seq;
				one_task->method = (*iter2).method;
				one_task->content_length = (*iter2).content_length;
				memcpy(one_task->content, (*iter2).content, one_task->content_length);

				task_size = sizeof(one_task_t)+one_task->content_length;
				fwrite(&task_size, sizeof(int), 1, fp);
				fwrite(buff, task_size, 1, fp);
				printf("write task id %d seq %d\n",one_task->transaction_id, one_task->seq);
			}
		}
	}

	fclose(fp);
	return;
}




