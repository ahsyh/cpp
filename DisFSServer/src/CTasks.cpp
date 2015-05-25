/*
 * CTasks.cpp
 *
 *  Created on: Nov 22, 2013
 *      Author: Shi Yihui
 */
#include <algorithm>

#include "CTasks.h"

bool COneTask::operator < (const COneTask& task){
	return seq < task.seq;
}

bool compare_task(const COneTask& task1, const COneTask& task2){
	return task1.seq < task2.seq;
}

int COneTransaction::add_one_task(
		enum method_e method_in,
		int trans_id,
		int seq_in,
		int length_in,
		char* buff)
{
	vector<COneTask>::iterator iter;

	for(iter=all_tasks.begin();iter!=all_tasks.end();iter++){
		if((*iter).seq == seq_in){
			return 1;
		}
	}

	COneTask onetask(method_in, trans_id, seq_in,length_in, buff);
	all_tasks.push_back(onetask);
	max_seq = (max_seq>seq_in)?max_seq:seq_in;
	seq_list.set(seq_in);
	return 0;
}

void COneTransaction::mysort(){
	sort(all_tasks.begin(),all_tasks.end(),compare_task);
}

void CTransactions::add_one_transaction(int trans_id){
	COneTransaction one_transaction(trans_id);
	all_transactions.push_back(one_transaction);
}

void CTransactions::del_one_transaction(int trans_id){
	COneTransaction* ptr = NULL;

	vector<COneTransaction>::iterator iter;
	for(iter=all_transactions.begin();iter!=all_transactions.end();iter++){
		if((*iter).transaction_id==trans_id){
			ptr = &(*iter);
			pthread_mutex_unlock(&(ptr->m_mutex));
			all_transactions.erase(iter);
			break;
		}
	}
}

COneTransaction* CTransactions::get_one_transaction(int trans_id){

	vector<COneTransaction>::iterator iter;

	for(iter=all_transactions.begin();iter!=all_transactions.end();iter++){
		if((*iter).transaction_id==trans_id){
			return (COneTransaction*)&(*iter);
		}
	}
	return NULL;
}
