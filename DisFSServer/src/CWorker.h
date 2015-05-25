/*
 * CWorker.h
 *
 *  Created on: Nov 21, 2013
 *      Author: Shi Yihui
 */

#ifndef CWRITER_H_
#define CWRITER_H_

#include <semaphore.h>
#include "message.h"
#include "CTasks.h"

class CWorker {
public:
	CWorker(int i);
	virtual ~CWorker();

	int m_no;
	bool m_free;

	sem_t m_work_sem;

	char m_name[32];
	int m_connfd;
	char msg_buff[128];

	void run();
	int handle_message(request_msg_t* msg);
	int commit_file_operation(COneTransaction *transaction);

};

#endif /* CWRITER_H_ */
