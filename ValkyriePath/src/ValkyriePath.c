/*
 ============================================================================
 Name        : ValkyriePath.c
 Author      : Shi Yihui
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true  1
#define false 0
typedef struct {
	int from;
	int to;
} subway_t;
typedef struct {
	int len;
	int path[25];
} spath_t;
subway_t g_subway[64];
int g_sw_num;
spath_t g_spath[25][25];

int g_result[100];
int g_res_num = 0xFFFF;
long g_cal = 0;

extern int print_res() ;

int get_sw() {
	FILE *fp;
	char str[256];
	char *ch1, *ch2;
	int i;
	fp = fopen("subway.txt", "r");
	if (fp == NULL)
		return -1;
	i = 0;
	g_sw_num = 0;
	while (fgets(str, 256, fp) != NULL) {
		if (strlen(str) < 2)
			continue;

		ch1 = ch2 = str;
		while (1) {
			while (*ch2 != '.')
				ch2++;
			*ch2 = '\0';
			g_subway[g_sw_num].from = i;
			g_subway[g_sw_num].to = atoi(ch1) - 1;
			g_sw_num++;

			ch2++;
			ch1 = ch2;
			if (*ch2 == '\n')
				break;
		}
		i++;
	}
	fclose(fp);
	return 0;
}
int exist_unknown_path(int *has_found, int vx) {
	int i;

	for (i = 0; i < 25; i++) {
		if ((i != vx) && (*(has_found + i) == 0))
			return true;
	}
	return false;
}
int get_next_vx_for_given_vx(int* list, int* l_num, int vx) {
	int i;
	int num = *l_num;
	num = 0;
	for (i = 0; i < g_sw_num; i++) {
		if (g_subway[i].from == vx) {
			list[num] = g_subway[i].to;
			num++;
		}

	}
	*l_num = num;
	return 0;
}
int get_next_turn(int* known_vx, int* l_r, int* l_w, int* has_found, int vx) {
	int list[25];
	int num;
	int i;
	int r_vx;
	int c_vx;

	int r, w;
	r = *l_r;
	w = *l_w;
	if (r >= w)
		return -1;
	r_vx = known_vx[r];
	get_next_vx_for_given_vx(list, &num, r_vx);
	for (i = 0; i < num; i++) {
		c_vx = list[i];
		if ((has_found[c_vx] != 1) && (c_vx != vx)) {
			known_vx[w] = c_vx;
			w++;
			g_spath[vx][c_vx].len = g_spath[vx][r_vx].len + 1;
			if (g_spath[vx][r_vx].len >= 0) {
				memcpy(g_spath[vx][c_vx].path, g_spath[vx][r_vx].path,
						g_spath[vx][r_vx].len * sizeof(int));
				g_spath[vx][c_vx].path[g_spath[vx][r_vx].len] = r_vx;
			}
			has_found[c_vx] = 1;
		}

	}
	r++;

	*l_r = r;
	*l_w = w;
	return 0;
}
int search_one_vertex(int vx) {
	int ret  = 0;
	int has_found[25];
	int i;
//	int j, k;
//	int turn;
	int known_vx[64];
	int k_r, k_w;

	for (i = 0; i < 25; i++) {
		has_found[i] = 0;
	}

	known_vx[0] = vx;
	k_r = 0;
	k_w = 1;
	g_spath[vx][vx].len = -1;
	while (exist_unknown_path(has_found, vx)) {
		if (get_next_turn(known_vx, &k_r, &k_w, has_found, vx) != 0)
			break;
	}
	return ret;
}
int search_sp() {
	int i;
	for (i = 0; i < 25; i++) {
		search_one_vertex(i);
	}
	return 0;
}
int print_all_sp() {
	int ret = 0;
	int i;
	int j;
	int k;
	for (i = 0; i < 25; i++) {
		printf("----------------------------\n");
		printf("Short path of room %2d\n", i + 1);
		for (j = 0; j < 25; j++) {
			if (j == i)
				continue;
			printf("-- room %2d to room %2d --:", i + 1, j + 1);

			printf("%2d ", i + 1);
			for (k = 0; k < g_spath[i][j].len; k++)
				printf("%2d ", g_spath[i][j].path[k] + 1);
			printf("%2d ", j + 1);
			printf("\n");
		}
	}
	return ret;
}
int get_next_room(int curr, int *list, int* next) {
	int i = curr + 1;
	if (curr == 25)
		return -1;
	while ((list[i] == -1) && (i <= 24)) {
		i++;
	}
	if (i <= 24)
		*next = i;
	else
		*next = 25;
	return 0;
}
int invalid_some_vx(int* visit, int next, int* path, int path_len) {
	int i, j;
	int found;

	for (i = next + 1; i < 25; i++) {
		found = 0;
		for (j = 0; j < path_len; j++) {
			if (visit[i] == path[j]) {
				found = 1;
				break;
			}
		}
		if (found)
			visit[i] = -1;
	}
	return 0;
}
int check_res(int* visit) {
	int l_visit[26];
	int l_result[100];
	int l_res_num;
	int path[25];
	int path_len;
	int curr, next;
	memcpy(l_visit, visit, 25 * sizeof(int));
	/*extend the result*/
	l_visit[25] = 0;
	l_result[0] = 0;
	l_res_num = 1;
	curr = 0;
	while (get_next_room(curr, l_visit, &next) == 0) {
		path_len = g_spath[l_visit[curr]][l_visit[next]].len;
		memcpy(path, g_spath[l_visit[curr]][l_visit[next]].path,
				path_len * sizeof(int));
		path[path_len] = l_visit[next];
		path_len++;
		invalid_some_vx(l_visit, next, path, path_len);
		memcpy(l_result + l_res_num, path, sizeof(int) * path_len);
		l_res_num += path_len;
		if (l_res_num >= g_res_num)
			return -1;
		curr = next;
	}

	if (l_res_num < g_res_num) {
		memcpy(g_result, l_result, sizeof(int) * l_res_num);
		g_res_num = l_res_num;
		print_res();
	}
	/*
	 curr=0;
	 next=2/curr;
	 */

	return 0;
}
int try(int* res, int pos, int* used) {
	int ret = 0;
	int i = 0;
	for (i = 24; i >= 1; i--) {
		if (used[i] == 0) {
			res[pos] = i;
			used[i] = 1;
			if (pos == 24) {
				check_res(res);
				g_cal += 1;
				/*
				 if(g_cal%10000000==0)
				 {
				 printf("current process: %d\n",g_cal);
				 }
				 */
			} else {
				try(res, pos + 1, used);
			}
			used[i] = 0;
		}
	}
	return ret;
}
int visit_all_list() {
	int used[25];
	int i;
	int result[26];

	for (i = 0; i < 25; i++) {
		used[i] = 0;
	}

	result[0] = 0;
	result[25] = 0;
	try(result, 1, used);

	return 0;
}
int print_res() {
	int i;
	printf("%d steps:\n", g_res_num);
	for (i = 0; i < g_res_num; i++) {
		printf("%2d ", g_result[i] + 1);
	}
	printf("\n");
	return 0;
}

int main() {
	get_sw();
	search_sp();
	//print_all_sp();
	visit_all_list();
	//print_res();
	return 0;
}

