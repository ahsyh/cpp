/*
 * linkoper.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: yihuishi
 */

#include <iostream>
#include <stdio.h>

using namespace std;

/*
 * Try to compile and run your code for all 4 questions.
 * Note: the maze problem -Question 4, may consume more time than you expect.
 *
 * State any assumptions.
 *
 * The skeleton code is provided to let you test your functions. You may
 * need to change it in order to get your code to work. (Your function needs
 * to work for all inputs, not just the ones in the skeleton code.)
 */


/* Question 1: Delete an item in a linked list */

typedef struct  list_item{
    int value;
    list_item *next;
} list_item;

void del_item(list_item **root, int val)
{
	if(root==NULL)
		return;
	if(*root==NULL)
		return;

	list_item* curr;
	list_item* prev;

	for(curr = *root, prev=NULL;curr!=NULL;){
		if(curr->value==val){
			if(prev==NULL)
				*root = curr->next;
			else
				prev->next = curr->next;

			delete(curr);
			break;
		}

		prev = curr;
		curr = curr->next;
	}

	return;
}

void print_list(list_item *root)
{
	list_item* lp=root;
	while(lp) {
		printf("%d\n", lp->value);
		lp=lp->next;
	}
}

void create_link(list_item** head, list_item* arr, int size)
{
	list_item* tmp;
	list_item* first=NULL;
	list_item* curr=NULL;

	for(int i=0;i<size;i++){
		tmp = new list_item;
		tmp->value = arr[i].value;
		if(curr!=NULL){
			curr->next = tmp;
		}
		curr = tmp;
		if(first==NULL){
			first = tmp;
		}
	}
	*head = first;
}

int testLinkOp()
{
	list_item list[6];

	list[0].value=1; list[0].next=list+1;
	list[1].value=2; list[1].next=list+2;
	list[2].value=3; list[2].next=list+3;
	list[3].value=4; list[3].next=list+4;
	list[4].value=5; list[4].next=list+5;
	list[5].value=6; list[5].next=0;

	printf("test to delete list item whose value is 3\n");
	list_item* head=NULL;
	create_link(&head, list,6);
	del_item(&head, 3);
	print_list(head);
	del_item(&head, 8);
	print_list(head);
	del_item(&head, 1);
	print_list(head);

	return 0;
}


