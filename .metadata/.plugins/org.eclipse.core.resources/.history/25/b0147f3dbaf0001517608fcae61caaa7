/*
 * student.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: yihuishi
 */


#include "CStudent.h"


#include <algorithm>
#include <vector>

using namespace std;

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

