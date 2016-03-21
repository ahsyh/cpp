/*
 * question03.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: yihuishi
 */

//============================================================================
// Name        : Q3.cpp
// Author      : syh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string.h>

using namespace std;



/* Question 3: Implement Base64 encoding API -
 *
 * DESCRIPTION
 *	Base64 processes input in 24bit chunks by converting each chunk into 4
 *	bytes of output. It does so by splitting input into four 6bit groups and
 *	using these as indexes in the following substitution table -
 *      const char base64_map[] =
 *        		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
 *       		"abcdefghijklmnopqrstuvwxyz"
 *      		"0123456789+/";
 *	If an input is not a multiple of 3 bytes, it's padded with zeros. In
 *	this case the output bytes that consist entirely of the pad data are
 *	replaced with '='.
 *
 * Example
 * 	An input of 0x00 0x45 0xF2 is equivalent to 00000000 01000101 11110010
 * 	bit sequence, which's then split into 000000 000100 010111 110010
 * 	and these are substituted to produce the following base64 encoding
 *      'A' 'E' 'X' 'y'
 *
 * RETURN VALUE
 * 	b64_length() should return output length for the input of 'len' bytes.
 * 	b64_encode() should encode 'len' bytes from 'src' into 'dst'. 'dst' is
 * 	guaranteed to hold at least b64_length('len') bytes.
 */

size_t b64_length(size_t len);
void b64_encode(const void * src, size_t len, void * dst);

size_t b64_length(size_t len){
	int multi = len/3;
	if((len%3)!=0){
		multi++;
	}
	return 4*multi;
}
static const char base64_map[] =
       		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
       		"abcdefghijklmnopqrstuvwxyz"
      		"0123456789+/";

void b64_encode(const void * src, size_t len, void * dst){
	unsigned char* d = (unsigned char*)dst;
	unsigned char* s = (unsigned char*)src;
	int s_index = 0;
	int d_index = 0;
	while((size_t)s_index < len){
		size_t s1,s2,s3;
		unsigned char c1,c2,c3;
		s1 = s_index;
		s2 = s_index+1;
		s3 = s_index+2;
		s_index+=3;

		c1=s[s1];

		if(s2>=len)
			c2=0;
		else
			c2=s[s2];

		if(s3>=len)
			c3=0;
		else
			c3=s[s3];

		d[d_index]  =base64_map[c1>>2];
		d[d_index+1]=base64_map[((c1&0x3)<<4)|(c2>>4)];
		d[d_index+2]=base64_map[((c2&0xF)<<2)|(c3>>6)];
		d[d_index+3]=base64_map[c3&0x3F];
		d_index+=4;

		if(s2>=len||s3>=len){
			d[d_index+2]='=';
			d[d_index+3]='=';
		}
	}
}


int testQ3() {
	cout << "MAZE" << endl; // prints MAZE
	int l;
	unsigned char b[] = {0x00,0x45,0xF2};
	l = b64_length(3);
	unsigned char *dst = new unsigned char[l];

	b64_encode(b,3,dst);

	return 0;
}



