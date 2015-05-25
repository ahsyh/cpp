/*
 * CSeqGenerator.h
 *
 *  Created on: Nov 21, 2013
 *      Author: Shi Yihui
 */

#ifndef CSEQGENERATOR_H_
#define CSEQGENERATOR_H_

#include <vector>
#include "CWorker.h"

using namespace std;

class CSeqGenerator {
public:
	CSeqGenerator();
	virtual ~CSeqGenerator();

	unsigned int m_curr_seq;
	vector<CWorker*> m_list;
	pthread_mutex_t m_mutex;

	int generator_seq();

};

#endif /* CSEQGENERATOR_H_ */
