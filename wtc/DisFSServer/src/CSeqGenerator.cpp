/*
 * CSeqGenerator.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: Shi Yihui
 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "CSeqGenerator.h"

CSeqGenerator::CSeqGenerator():
	m_curr_seq(10), m_mutex(PTHREAD_MUTEX_INITIALIZER) {

}

CSeqGenerator::~CSeqGenerator() {
}

int CSeqGenerator::generator_seq(){
	pthread_mutex_lock(&m_mutex);
	int n = m_curr_seq;
	m_curr_seq++;
	if(m_curr_seq<10)m_curr_seq=10;
	pthread_mutex_unlock(&m_mutex);
	return n;
}
