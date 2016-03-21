//============================================================================
// Name        : maze.cpp
// Author      : syh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stack>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

/* Question 4: Maze Problem (Bonus)
 * Starting point is m[0][0], need to find a path go to m[9][9]. 0 means OK,
 * 1 means cannot go there, boundary is 0 and 9, cannot go beyond boundary.
 * Each step can be made horizontally or vertically for one more grid (diagonal
 * jump is not allowed).
 * Your program should print a series of grid coordinates that start from m[0][0]
 * and go to m[9][9]
 * Hint: No need to find the shortest path, only need to find one path that gets
 * you to desitination.
 */

static int m[10][10] =
{   0,    1,    1,    0,    1,    1,    1,    1,    0,    1,
    0,    1,    0,    0,    0,    0,    1,    1,    1,    1,
    0,    1,    0,    0,    1,    0,    1,    1,    1,    1,
    0,    0,    0,    1,    1,    0,    1,    1,    1,    1,
    0,    1,    1,    1,    0,    0,    1,    0,    0,    1,
    1,    1,    1,    1,    0,    1,    1,    0,    0,    1,
    1,    1,    0,    1,    0,    0,    0,    0,    0,    1,
    1,    1,    0,    0,    1,    1,    1,    1,    0,    1,
    0,    1,    1,    0,    1,    1,    1,    1,    0,    1,
    0,    1,    1,    1,    1,    1,    1,    1,    0,    0
};


typedef enum {
	NONE=0,
	RIGHT,
	DOWN,
	LEFT,
	UP
}direct;

typedef struct{
	int x;
	int y;
	int dir;
}action_t;

bool validPos(int m[10][10], int size_x, int size_y, int x, int y){
	bool ret = true;

	if(x<0||x>=size_x)
		ret=false;
	else if(y<0||y>=size_y)
		ret=false;
	else if(m[x][y]!=0)
		ret=false;

	return ret;
}

bool searchMaze(int m[10][10], int size_x, int size_y){
	bool ret = false;
	stack<action_t> s;

	action_t tmp;

	tmp.x = 0;
	tmp.y = 0;
	tmp.dir = NONE;

	s.push(tmp);
	m[0][0] = 3;

	while(!s.empty()){
		tmp = s.top();
		s.pop();
		if(tmp.x==(size_x-1)&&tmp.y==(size_y-1)){
			s.push(tmp);
			ret = true;
			break;
		}

		for(;tmp.dir<UP;){
			tmp.dir++;

			int x,y,off_x, off_y;
			switch(tmp.dir){
			case RIGHT:
				off_x = 0;
				off_y = 1;
				break;
			case DOWN:
				off_x = 1;
				off_y = 0;
				break;
			case LEFT:
				off_x = 0;
				off_y = -1;
				break;
			case UP:
				off_x = -1;
				off_y = 0;
				break;
			}
			x = tmp.x + off_x;
			y = tmp.y + off_y;

			if(validPos(m,size_x, size_y,x,y)){
				s.push(tmp);
				tmp.dir=NONE;
				tmp.x=x;
				tmp.y=y;
				s.push(tmp);
				m[x][y]=3;
				break;
			}

			if(tmp.dir==UP)
				m[tmp.x][tmp.y]=0;
		}
	}

	if(ret){
		vector<action_t> v;

		while(!s.empty()){
			action_t tmp = s.top();
			s.pop();
			v.push_back(tmp);
		}

		vector<action_t>::iterator it;
		reverse(v.begin(),v.end());
		for(it=v.begin();it!=v.end();it++){
			printf("(%d,%d)\n",it->x,it->y);
		}

		for(int i=0;i<size_x;i++){
			for(int j=0;j<size_y;j++){
				int val = m[i][j];
				if(val==3)
					printf("# ");
				else
					printf("%d ",val);
			}
			printf("\n");
		}
	}

	return ret;
}

int testMaze() {
	cout << "MAZE PATH:" << endl; // prints MAZE
	searchMaze(m, 10, 10);
	return 0;
}
