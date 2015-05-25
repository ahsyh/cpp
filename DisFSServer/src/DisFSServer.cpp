//============================================================================
// Name        : DisFSServer.cpp
// Author      : Shi Yihui
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <getopt.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

#include "CWorker.h"
#include "CManager.h"
#include "CTasks.h"
#include "CSeqGenerator.h"
#include "CParameter.h"
#include "message.h"

using namespace std;

const int 		MAX_WRITER_NUM=64;
CTransactions 	g_transactions;
bool 			g_quit = false;
CSeqGenerator* 	g_seq_gen = NULL;
CParameter 		g_para;
pthread_mutex_t	g_file_protection_mutex = PTHREAD_MUTEX_INITIALIZER;

void workthread(CWorker* worker){
	worker->run();
}

extern void read_transaction();
extern void write_transaction();

int main() {
	if(g_para.read_para_file()!=0)
	{
		printf("Read ini file failed, please check it exists and content correct.\n");
	}

    vector<CWorker*> worker_list;
    vector<thread> ths;

    g_seq_gen = new CSeqGenerator();
    read_transaction();

    for(int i=0;i<MAX_WRITER_NUM;i++){
    	CWorker *worker = new CWorker(i);
    	worker_list.push_back(worker);
    	ths.push_back(thread(&workthread,worker));
    }

    CManager* manager = new CManager(worker_list);
    manager->run();

    for (auto& th : ths) {
        th.join();
    }

	vector<CWorker*>::iterator iter;
	for(iter=worker_list.begin();iter!=worker_list.end();iter++){
		delete(*iter);
	}

    write_transaction();
    delete(g_seq_gen);

    return 0;
}
