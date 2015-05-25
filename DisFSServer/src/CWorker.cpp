/*
 * CWorker.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: Shi Yihui
 */
#include <stdio.h>
#include <unistd.h>

extern "C"{
#include  "unp.h"
}

#include <vector>

#include "CWorker.h"
#include "message.h"
#include "CSeqGenerator.h"
#include "CTasks.h"
#include "CParameter.h"

using namespace std;

extern bool g_quit;
extern CParameter g_para;
extern CSeqGenerator* g_seq_gen;
extern CTransactions g_transactions;

CWorker::CWorker(int i):
		m_no(i),m_free(true),m_connfd(-1)
{
	sem_init(&m_work_sem, 0, 0);
	snprintf(m_name, 32, "wk-%03d",m_no);
}

CWorker::~CWorker() {
	sem_destroy(&m_work_sem);
}

extern pthread_mutex_t g_file_protection_mutex;

int CWorker::commit_file_operation(COneTransaction *transaction){
	transaction->mysort();
	char filename[256];
	int result = 0;
	int n;

	//all file operation will be handled one by one
	pthread_mutex_lock(&g_file_protection_mutex);
	snprintf(filename,256,"%s/%s",g_para.base_dir,transaction->file_name);
	printf("%s: prepare write file %s\n",m_name, filename);
	FILE* fp = fopen(filename,"a+");

	if(fp!=NULL){
		vector<COneTask>::iterator iter;

		for(iter=transaction->all_tasks.begin();
			iter!=transaction->all_tasks.end();
			iter++)
		{
			n = fwrite((*iter).content,(*iter).content_length,1,fp);
			if(n!=1){
				printf("%s: write action has some content missing\n", m_name);
				result = -1;
				break;
			}
			else
			{
				printf("%s: write file seq %d\n", m_name, (*iter).seq);
			}
		}

		fclose(fp);
	}
	else
	{
		printf("open file failed\n");
		result = -1;
	}
	pthread_mutex_unlock(&g_file_protection_mutex);
	return result;
}
// return value:
//  0: continue wait this conn fd
int CWorker::handle_message(request_msg_t* msg){
	int n;
	FILE* fp=NULL;

	//just avoid modify all variables below
	COneTransaction* l_one_trans=NULL;
	COneTransaction** one_trans=&l_one_trans;

	switch(msg->method){
    case READ:
    	char l_filename[128];
    	snprintf(l_filename, 128, "%s/%s", g_para.base_dir, msg->content);
		printf("%s: read operation, file :%s:\n", m_name, msg->content );

		fp=fopen(l_filename, "r");
    	if(fp==NULL){
			reply_error_msg(m_connfd, 206, msg->transaction_id, msg->seq_num);
    	}
    	else{
			reply_ack_msg(m_connfd, msg->transaction_id);
	    	fclose(fp);
    	}
    	break;
    case NEW_TXN:
    	n = g_seq_gen->generator_seq();
    	g_transactions.add_one_transaction(n);
    	*one_trans = g_transactions.get_one_transaction(n);
    	memcpy((*one_trans)->file_name,msg->content,msg->content_length);
    	printf("%s: new_txn trans %d, file name :%s:\n",m_name, n, (*one_trans)->file_name);
    	reply_ack_msg(m_connfd,n);
    	break;
    case WRITE:
    	{
    		*one_trans = g_transactions.get_one_transaction(msg->transaction_id);
    		if(*one_trans == NULL){
    			reply_error_msg(m_connfd, 201, msg->transaction_id, msg->seq_num);
    			break;
    		}
    		if(msg->seq_num == 0){
    			reply_error_msg(m_connfd, -10, msg->transaction_id, msg->seq_num);
    			break;
    		}

    		if(pthread_mutex_trylock(&(*one_trans)->m_mutex)!=0){
    			*one_trans = NULL;
    			reply_error_msg(m_connfd, -7, msg->transaction_id, msg->seq_num);
    			break;
    		}
    	}
    	if(*one_trans != NULL)
    	{
    		if(msg->transaction_id!=(*one_trans)->transaction_id)
    		{
    			reply_error_msg(m_connfd, 201, msg->transaction_id, msg->seq_num);
    			break;
    		}

    		printf("%s: add write operation, trans %d seq %d\n", m_name, msg->transaction_id, msg->seq_num );
    		int add_result = (*one_trans)->add_one_task(
					msg->method,
					msg->transaction_id,
					msg->seq_num,
					msg->content_length,
					msg->content);

    		if(add_result==1){
    			//seq duplicate
    			reply_error_msg(m_connfd, -8, msg->transaction_id, msg->seq_num);
    		}
    		else if((*one_trans)->is_commited)
			{
				if( ((*one_trans)->max_seq) != (int)(*one_trans)->seq_list.count() )
				{
					printf("%s: check in write, some seq miss\n", m_name);
//					//send resend req
//					for(int i=1;i<=(*one_trans)->max_seq;i++){
//						if(!(*one_trans)->seq_list.test(i)){
//							reply_resend_msg(m_connfd, (*one_trans)->transaction_id, i);
//						}
//					}
				}
				else{
					printf("commit in write case\n");

					int result = commit_file_operation(*one_trans);

					if(result!=0){
		    			reply_error_msg(m_connfd, 205, msg->transaction_id, msg->seq_num);
					}
					else{
						reply_ack_msg(m_connfd, (*one_trans)->transaction_id);
						g_transactions.del_one_transaction((*one_trans)->transaction_id);
					}

					*one_trans = NULL;
				}
			}

    		if(*one_trans!=NULL)
			{
	    		pthread_mutex_unlock(&(*one_trans)->m_mutex);
				*one_trans = NULL;
			}
    	}
    	break;
    case COMMIT:
    	if(*one_trans == NULL)
    	{
    		*one_trans = g_transactions.get_one_transaction(msg->transaction_id);
    		if(*one_trans == NULL){
    			reply_error_msg(m_connfd, 201, msg->transaction_id, msg->seq_num);
    			break;
    		}

    		if(pthread_mutex_trylock(&(*one_trans)->m_mutex)!=0){
    			*one_trans = NULL;
    			reply_error_msg(m_connfd, -7, msg->transaction_id, msg->seq_num);
    		}
    	}
    	if(*one_trans != NULL)
    	{
    		if(msg->transaction_id!=(*one_trans)->transaction_id)
    		{
    			reply_error_msg(m_connfd, 201, msg->transaction_id, msg->seq_num);
    			break;
    		}

    		printf("%s: set commit to true, trans %d\n",m_name,(*one_trans)->transaction_id);
    		(*one_trans)->is_commited = true;
			if( ((*one_trans)->max_seq) != (int)(*one_trans)->seq_list.count() )
			{
				//send resend req
				for(int i=1;i<=(*one_trans)->max_seq;i++){
					if(!(*one_trans)->seq_list.test(i)){
						reply_resend_msg(m_connfd, (*one_trans)->transaction_id, i);
					}
				}
			}
			else{
				int result = commit_file_operation(*one_trans);
				if(result!=0){
	    			reply_error_msg(m_connfd, 205, msg->transaction_id, msg->seq_num);
				}
				else{
					reply_ack_msg(m_connfd, (*one_trans)->transaction_id);
					g_transactions.del_one_transaction((*one_trans)->transaction_id);
				}

				*one_trans = NULL;
			}
			if(*one_trans!=NULL)
			{
	    		pthread_mutex_unlock(&(*one_trans)->m_mutex);
				*one_trans = NULL;
			}
    	}
    	break;
    case ABORT:
    	if(msg->transaction_id == -1){
    		printf("%s: client ask to quit\n",m_name);
    		g_quit = true;
    		break;
    	}
    	if(*one_trans == NULL)
    	{
    		printf("%s: abort operation, trans: %d\n", m_name, msg->transaction_id );
    		*one_trans = g_transactions.get_one_transaction(msg->transaction_id);
    		if(*one_trans == NULL){
    			reply_error_msg(m_connfd, 201, msg->transaction_id, msg->seq_num);
    			break;
    		}

    		if(pthread_mutex_trylock(&(*one_trans)->m_mutex)!=0){
    			*one_trans = NULL;
    			reply_error_msg(m_connfd, -7, msg->transaction_id, msg->seq_num);
    		}
    	}
    	if(*one_trans != NULL)
    	{
    		if(msg->transaction_id!=(*one_trans)->transaction_id)
    		{
    			reply_error_msg(m_connfd, 201, msg->transaction_id, msg->seq_num);
    			break;
    		}

			reply_ack_msg(m_connfd, (*one_trans)->transaction_id);
			g_transactions.del_one_transaction((*one_trans)->transaction_id);
			*one_trans = NULL;

			if(*one_trans!=NULL)
			{
	    		pthread_mutex_unlock(&(*one_trans)->m_mutex);
				*one_trans = NULL;
			}
    	}
    	break;
    default:
    	//reply_error_msg(connfd, -6, msg->transaction_id, msg->seq_num);
    	break;
    }
    return 0;
}

void CWorker::run() {
	int time_out_counter = 0;

	printf("%s: start\n",m_name);
	while(1)
	{
		sem_wait(&m_work_sem);

		if(g_quit){
			printf("%s: end\n",m_name);
			return;
		}
		printf("%s: begin to work\n",m_name);

		char buff[MAXLINE];
		request_msg_t *msg = (request_msg_t*)buff;
		time_out_counter = 0;

		for(;;){
			memset(buff,0x00,MAXLINE);

			int n = get_next_message(m_connfd, msg, MAXLINE, m_name);

			if( n == -255 ){
				printf("%s: client close connection\n",m_name);
				Close(m_connfd);
				break;
			}
			else if( n == 0 ){
				time_out_counter ++;
				if(g_quit){
					Close(m_connfd);
					printf("%s: quit now\n",m_name);
					return;
				}
				if(	time_out_counter > 10 ){
					time_out_counter = 0;
					printf("%s: client don't operate for long time.\n",m_name);
				}
				continue;
			}
			else if(n<0)
			{
				reply_error_msg(m_connfd, n, 0, 0);
				continue;
			}
			time_out_counter = 0;

			printf("%s: get message, %d %d %d %d\n",
				m_name,
				msg->method,msg->transaction_id,
				msg->seq_num,msg->content_length);

			handle_message(msg);

		}
		printf("%s: free again\n",m_name);
		m_free = true;
	}

}

