/*
 * CStudent.h
 *
 *  Created on: Mar 23, 2016
 *      Author: yihuishi
 */

#ifndef CSTUDENT_H_
#define CSTUDENT_H_

#include <string>
#include <string.h>

using namespace std;

class CStudent {
public:
	CStudent(string n, int a){
		// TODO Auto-generated constructor stub
		strncpy(m_Name,n.c_str(),8);
		m_Age = a;
	};
	virtual ~CStudent(){};

	char m_Name[8];
	int m_Age;
};


#endif /* CSTUDENT_H_ */
