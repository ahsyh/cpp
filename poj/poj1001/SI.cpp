/*
 * SI.cpp
 *
 *  Created on: 2013Äê11ÔÂ18ÈÕ
 *      Author: yihuishi
 */


#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>

//#define _TEST_ 0

using namespace std;

const int MAX_BIT_NUMBER=256;

//Super interger class
class SI {
public:
	int point_pos;
	int length;
	unsigned char digitals[MAX_BIT_NUMBER];

	SI& operator=(int i);
	SI& operator=(SI& tmp);
	SI& operator+(SI& tmp);
	SI& operator*(int i);
	SI& operator*(SI& tmp);

	void print();
	void adjust();

public:
	SI();
	SI(int i);
	SI(char* str);
	SI(SI& tmp);
	virtual ~SI();
};

SI::SI():point_pos(0),length(1) {
	// TODO Auto-generated constructor stub
	memset(digitals,0,MAX_BIT_NUMBER*sizeof(unsigned char));
}

SI::SI(int i):point_pos(0),length(0) {
	memset(digitals,0,MAX_BIT_NUMBER*sizeof(unsigned char));
	while(i!=0)
	{
		digitals[length] = i%10;
		i /= 10;
		length++;
	}
}

void SI::adjust()
{
	//eliminate the latest 0 after point
	int i=0;
	for(i=0;i<point_pos;i++)
	{
		if(digitals[i]!=0)break;
	}
	if(i!=0)
	{
		for(int j=0;j<length;j++)
		{
			if(j<length-i)
				digitals[j]=digitals[j+i];
			else
				digitals[j]=0;
		}
		length -= (i);
		point_pos -= i;
	}

	for(i=0;i<length;i++)
	{
		if(digitals[length-i-1]!=0)break;
	}

	if(i!=0)
	{
		length -= i;
	}
	if(length==0)
		length = 1;
}

SI::SI(char* str):point_pos(0),length(0) {
	memset(digitals,0,MAX_BIT_NUMBER*sizeof(unsigned char));

	int new_length = strlen(str);
	length=0;
	for(int i=0;i<new_length;i++){
		if(str[new_length-i-1]=='.')
			point_pos = length;
		else
		{
			digitals[length]=str[new_length-i-1]-'0';
			length++;
		}
	}
	adjust();
}

SI::SI(SI& tmp):point_pos(tmp.point_pos),length(tmp.length){
	// TODO Auto-generated constructor stub
	memcpy(digitals,tmp.digitals,MAX_BIT_NUMBER*sizeof(unsigned char));
}

SI::~SI() {
	// TODO Auto-generated destructor stub
}

SI& SI::operator=(int i) {
	// TODO Auto-generated destructor stub
	point_pos = 0;
	length = 0;
	memset(digitals,0,MAX_BIT_NUMBER*sizeof(unsigned char));
	while(i!=0)
	{
		digitals[length] = i%10;
		i /= 10;
		length++;
	}
	return *this;
}

SI& SI::operator=(SI& tmp) {
	// TODO Auto-generated destructor stub
	point_pos = tmp.point_pos;
	length = tmp.length;
	memcpy(digitals,tmp.digitals,MAX_BIT_NUMBER*sizeof(unsigned char));
	return *this;
}

SI& SI::operator+(SI& tmp) {
	// TODO Auto-generated destructor stub
	SI result(*this);
	//point_pos = tmp.point_pos;

	int new_length = (length > tmp.length)?length:tmp.length;
	int jinwei=0;
	result.length = new_length;
	for(int i=0;i<(new_length);i++){
		result.digitals[i]=digitals[i]+tmp.digitals[i]+jinwei;
		jinwei = result.digitals[i] / 10;
		result.digitals[i] = result.digitals[i] % 10;
	}

	if(jinwei!=0)
	{
		result.digitals[new_length]=jinwei;
		result.length = new_length+1;
	}

	return result;
}

SI& SI::operator*(int i) {
	SI result;

	if(i==0) return result;
	if(length==1&&digitals[0]==0) return result;
	if(i==10) {
		result.digitals[0]=0;
		for(int j=0;j<length;j++){
			result.digitals[j+1]=digitals[j];
		}
		result.length = length+1;
		return result;
	}

	int jinwei=0;
	result.length = length;
	for(int j=0;j<length;j++){
		result.digitals[j]=digitals[j]*i+jinwei;
		jinwei = result.digitals[j] / 10;
		result.digitals[j] = result.digitals[j] % 10;
	}

	if(jinwei!=0){
		result.digitals[result.length]=jinwei;
		result.length = length+1;
	}

	return result;
}

SI& SI::operator*(SI& tmp) {
	// TODO Auto-generated destructor stub
	SI result;
	SI temp_val1(*this);
	SI temp_val2;

	for(int i=0;i<tmp.length;i++){
		int j=tmp.digitals[i];
		temp_val2 = temp_val1*j;
		result = result+temp_val2;
		temp_val1=temp_val1*10;
	}
	result.point_pos = point_pos + tmp.point_pos;
	//point_pos = tmp.point_pos;

	result.adjust();

	return result;
}

void SI::print(){
	if(length<=point_pos)
	{
		printf(".");

		for(int i=0;i<point_pos-length;i++){
			printf("0");
		}
		for(int i=0;i<length;i++){
			printf("%d",digitals[length-i-1]);
		}
		printf("\n");

		return;
	}
	int i;

	for(i=0;i<length-point_pos;i++){
		printf("%d",digitals[length-i-1]);
	}
	if(point_pos!=0)
	{
		printf(".");
		for(;i<length;i++){
			printf("%d",digitals[length-i-1]);
		}
	}
	printf("\n");
	return;
}

void cal_exponent(char* s1,int d){
	if(d==0)
	{
		printf("1\n");
		return;
	}

	SI result(1);
	SI tmp_val(s1);

	while(d!=0)
	{
		if(d&0x1)
			result = result*tmp_val;
		tmp_val = tmp_val*tmp_val;
		d=d>>1;
	}
	result.print();
}

int main(void)
{
#ifdef _TEST_
	FILE* fp=fopen("D:/workspace/poj1001/Debug/input.txt","r");
#endif

	char s1[128];
	int d;


#ifdef _TEST_
	while(fscanf(fp,"%s%d",s1,&d)!=EOF)
#else
	while(scanf("%s%d",s1,&d)!=EOF)
#endif
	{
		s1[6] = '\0';
		cal_exponent(s1,d);
	}

	return 0;
}
