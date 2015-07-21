//============================================================================
// Name        : FB_003.cpp
// Author      : syh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <sys/time.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

typedef struct one_guess{
	vector<int> guess;
	int score;
}one_guess;

typedef struct one_case{
	vector<one_guess> guesslist;
	int n;
	int k;
	int q;
}one_case;

vector<one_case> g_cases;

int read_one_case(
		istream &file,
		vector<one_case> &cases)
{
	int n,k,q;
	file >> n >> k >> q;

	one_case one;

	one.n = n;
	one.k = k;
	one.q = q;

	one_guess aguess;
	for(int i=0;i<q;i++){
		aguess.guess.clear();
		for(int j=0;j<k;j++){
			int tmp;
			file >> tmp;
			aguess.guess.push_back(tmp);
		}
		int score;
		file >> score;
		aguess.score = score;
//		vector<one_guess>::iterator it=one.guesslist.begin();
//		for(;it!=one.guesslist.end();it++){
//			if(abs(score<<1-k) > abs(it->score<<1-k))
//				break;
//		}
//		one.guesslist.insert(it,aguess);
		one.guesslist.push_back(aguess);
	}
	cases.push_back(one);

	return 0;
}

int read_para(istream &f){
	int t_num;
	f >> t_num;

	for(int i=0;i<t_num;i++){
		read_one_case(f, g_cases);
	}
	return 0;
}

bool check_validation(
		one_case &tcase,int* select,
		vector<map<int,int> >& excludes,
		int curr,
		vector<int> &poss)
{
	int n,k;

	n = tcase.n;
	k = tcase.k;

	for(int i=0;i<k;i++){
		int tmp = tcase.guesslist[curr].guess[i];
		if(select[i]!=0&&poss[i]==1){
			if(select[i]!=tmp)
				return false;
		}
		else if(select[i]!=0&&poss[i]==0){
			if(select[i]==tmp)
				return false;
		}
		else if(select[i]==0&&poss[i]==1){
			if(excludes[i][tmp]!=0)
				return false;
		}
		else if(select[i]==0&&poss[i]==0){
			int j;
			for(j=1;j<=n;j++){
				if(tmp==j||(excludes[i][j]>0)){

				}
				else
					break;
			}
			if(j>=(n+1))
				return false;
		}
	}
	return true;
}

void update_possible(
		one_case &tcase,
		int* select, int* tselect,
		vector<map<int,int> >& excludes,
		int curr,vector<int> &poss)
{
	int k;

	k = tcase.k;

	for(int i=0;i<k;i++){
		tselect[i] = select[i];
		int tmp = tcase.guesslist[curr].guess[i];

		if(poss[i]==1){
			select[i]=tmp;
		}
		else{
			excludes[i][tmp]++;
		}
	}

}

void clear_possible(
		one_case &tcase,
		int* select, int* tselect,
		vector<map<int,int> >& excludes,
		int curr,vector<int> &poss)
{
	int k;
	k=tcase.k;

	for(int i=0;i<k;i++){
		select[i] = tselect[i];
		int tmp = tcase.guesslist[curr].guess[i];

		if(poss[i]==1){
		}
		else{
			excludes[i][tmp]--;
		}
	}
}

void recur_poss(int k, int score, int used,
		int curr,vector<int> &one,
		vector<vector<int> > &possible)
{
	if(used>=score)
	{
		possible.push_back(one);
//		for(int i=0;i<(int)one.size();i++){
//			cout << one[i] <<",";
//		}
//		cout << endl;
		return;
	}

	if((k-curr)<(score-used))
		return;

	one[curr] = 1;
	recur_poss(k,score,used+1,curr+1,one, possible);
	one[curr] = 0;
	recur_poss(k,score,used,  curr+1,one, possible);
}

void generate_possible(int score, int k,
		vector<vector<int> > &possible)
{
	vector<int> one;

	for(int i=0;i<k;i++){
		one.push_back(0);
	}
	recur_poss(k,score, 0, 0,one,possible);
}

bool test(
		one_case &tcase,
		int* select,
		vector<map<int,int> > &excludes,
		int curr)
{

	int k,q;
	q = tcase.q;
	k = tcase.k;

	if(curr>=q){
#ifdef _TEST_
		for(int i=0;i<k;i++){
			cout << select[i] << ",";
		}
		cout << endl;
#endif
		return true;
	}

	vector<vector<int> > possible;
	generate_possible(tcase.guesslist[curr].score, k, possible);

	bool ret=false;

	for(int i=0;i<(int)possible.size();i++){
		if(check_validation(tcase,select, excludes,curr,possible[i])){
			int tselect[k];
			update_possible(tcase,select, tselect,
					excludes,curr,possible[i]);
			ret = test(tcase,select, excludes, curr+1);
			clear_possible(tcase,select, tselect,
					excludes,curr,possible[i]);
		}
		if(ret)
			return true;
	}

	return false;
}

void init_exclude(
		one_case &tcase,
		int* select,
		vector<map<int, int> > &excludes)
{
	int n, k;

	n = tcase.n;
	k = tcase.k;

	for(int i=0;i<k;i++){
		select[i]=0;
		map<int,int> m;
		for(int j=1;j<=n;j++){
			m.insert(make_pair(j,0));
		}
		excludes.push_back(m);
	}
}

void check_one_case(one_case &tcase){
	int select[tcase.k];
	vector<map<int, int> > excludes;
	init_exclude(tcase, select, excludes);

	if(	test(tcase, select, excludes, 0) )
		cout << "Yes" << endl;
	else
		cout << "No" << endl;
}

void check_cases(){
#ifdef _TEST_
	struct timeval otv, ctv;
	struct timezone tz;
	gettimeofday(&otv, &tz);
#endif
	for(int i=0;i<(int)g_cases.size();i++ ){
		check_one_case(g_cases[i]);
#ifdef _TEST_
		gettimeofday(&ctv, &tz);
		if(ctv.tv_usec > otv.tv_usec)
			cout << "used " << ctv.tv_sec - otv.tv_sec << " sec and "
				<< ctv.tv_usec - otv.tv_usec << " usec" << endl;
		else
			cout << "used " << ctv.tv_sec - otv.tv_sec - 1 << " sec and "
				<< 1000000 + ctv.tv_usec - otv.tv_usec << " usec" << endl;

		otv = ctv;
#endif
	}
}

int main() {
#ifdef _TEST_
	ifstream file;
	file.open("input001.txt");

	read_para(file);
#else
	read_para(cin);
#endif
	check_cases();

	return 0;
}
