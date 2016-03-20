/*
 * CTasks.h
 *
 *  Created on: Nov 22, 2013
 *      Author: Shi Yihui
 */

#ifndef CTASKS_H_
#define CTASKS_H_

#include <vector>
#include <bitset>
#include <pthread.h>
#include <string.h>
#include "message.h"

using namespace std;

const int MAX_FILE_NAME=128;

class COneTask {
public:
	COneTask(enum method_e method_in, int trans_id, int seq_in,int length_in, char* buff):
		method(method_in),
		transaction_id(trans_id),
		seq(seq_in),
		content_length(length_in)
	{
		memcpy(content, buff, length_in);
	};
	virtual ~COneTask(){};

	bool operator < (const COneTask& task);

	enum method_e method;
	int transaction_id;
	int seq;
	int content_length;
	char content[1024];
};

class COneTransaction {
public:
	COneTransaction(int trans_id):
		transaction_id(trans_id),max_seq(0),
		m_mutex(PTHREAD_MUTEX_INITIALIZER),
		is_commited(false)
	{
		memset(file_name, 0x00, MAX_FILE_NAME*sizeof(char));

	};
	virtual ~COneTransaction(){
		all_tasks.clear();
	};

	int transaction_id;
	bitset<64> seq_list;
	int max_seq;
	pthread_mutex_t m_mutex;
	bool	is_commited;
	char	file_name[MAX_FILE_NAME];
	vector<COneTask> all_tasks;

	int add_one_task(enum method_e method_in, int trans_id, int seq_in,int length_in, char* buff);
	void mysort();
};

class CTransactions {
public:
	CTransactions(){};
	virtual ~CTransactions(){all_transactions.clear();};

	vector<COneTransaction> all_transactions;

	void add_one_transaction(int trans_id);
	void del_one_transaction(int trans_id);
	COneTransaction* get_one_transaction(int trans_id);
};

#endif /* CTASKS_H_ */
