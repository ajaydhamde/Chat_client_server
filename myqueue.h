#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__
struct node{
	struct node *next;
	int *cli_socket;
	
};
typedef struct node  node_t;
/*	fun prototype	*/
void enqueue(int *);
int *dequeue();
#endif
