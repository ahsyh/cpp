/*
 * question02.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: yihuishi
 */


//============================================================================
// Name        : Q2.cpp
// Author      : syh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

/* Question 2: write a binary search function on a sorted ascending int array.
   return the index of the element.
   eg:
   int t[] = [1,3,5,7,9];
   find index of 5.
*/
int bsearch(int *table, int sz, int key)
{
	int l, r, mid;
	int ret=-1;

	l=0;
	r=sz-1;

	if(table[l]==key)
	{
		return l;
	}
	if(table[r]==key)
	{
		return r;
	}

	while(l<r){
		mid = (l+r)/2;

		if(mid==l||mid==r){
			break;
		}

		if(table[mid]==key)
		{
			ret = mid;
			break;
		}

		if(table[mid]>key){
			r = mid;
		}
		else{
			l = mid;
		}
	}


	return ret;
}


int testQ2() {
	cout << "MAZE" << endl; // prints MAZE

	int t[] = {1,3,5,7,9};
	cout << bsearch(t,5,5);
	return 0;
}

