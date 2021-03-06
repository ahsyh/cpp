/*
 * student.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: yihuishi
 */


#include <algorithm>
#include <vector>

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

int cmp( const CStudent &a, const CStudent &b ){
    if( a.m_Age < b.m_Age )
       return 1;
    else
       if( a.m_Age == b.m_Age )
    	   return 0;
    else
       return -1;
}

int testCmp(){

	vector< CStudent > list;
	CStudent* a1 = new CStudent("SYH",38);
//	CStudent* a2 = new CStudent("HLN",37);
//	CStudent* a3 = new CStudent("SHB",36);
//
	list.push_back(*a1);
//	list.push_back(*a2);
//	list.push_back(*a3);
//	list.push_back(new CStudent("SYH",38));
//	list.push_back(new CStudent("HLN",37));
//	list.push_back(new CStudent("SHB",36));

	sort(list.begin(),list.end(),cmp);

	return 0;
}

