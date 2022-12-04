#include <stdio.h>
#include <stdlib.h>
#include "myqueue.h"

node_t *head = NULL;
node_t *tail = NULL;
void enqueue(int *cli_sock){
	node_t *new_node = malloc(sizeof(node_t));
	new_node->cli_socket =  cli_sock;
	new_node->next = NULL;
	if(tail == NULL){
		head = new_node;

	}
	else{
		tail->next = new_node;
	}
	tail = new_node;
}
int *dequeue(){
	if(head == NULL){
		return NULL;
	}else{
		int *result = head->cli_socket;
		node_t *temp = head;
		head = head->next;
		if(head == NULL){	
			tail = NULL;	
		}
		free(temp);
		return result;
	}
}
