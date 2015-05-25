/*
 * CParameter.h
 *
 *  Created on: Nov 23, 2013
 *      Author: Shi Yihui
 */

#ifndef CPARAMETER_H_
#define CPARAMETER_H_

class CParameter {
public:
	CParameter();
	virtual ~CParameter();

	int server_port;
	char base_dir[128];
	int read_para_file();
};

#endif /* CPARAMETER_H_ */
