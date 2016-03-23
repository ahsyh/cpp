/*
 * bits.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: yihuishi
 */

#include <iostream>
using namespace std;

int bits(unsigned char c){
	int r=0;
	while(c!=0){
		c&=c-1;
		r++;
	}
	return r;
}

int testBits() {

	cout << "MAZE" << endl; // prints MAZE
	for(int i=0;i<256;i++){
		cout << bits(i) << ", ";
	}
	return 0;
}


