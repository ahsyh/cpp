/*
 * CManager.h
 *
 *  Created on: Nov 21, 2013
 *      Author: Shi Yihui
 */

#ifndef CMANAGER_H_
#define CMANAGER_H_

#include <vector>
#include "CWorker.h"

using namespace std;

class CManager {
public:
	CManager(vector<CWorker*> worker_list);
	virtual ~CManager();

	vector<CWorker*> m_list;
	char m_name[32];

	void run();
	void notice_quit();
	CWorker* reserve_free_writer();
	CWorker* search_given_writer(int transaction_id);


};

#endif /* CMANAGER_H_ */
