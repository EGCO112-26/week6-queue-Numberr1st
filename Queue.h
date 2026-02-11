#include <stdio.h>
#include <stdlib.h>
#include "Node.h"

typedef struct {
	 NodePtr headPtr,tailPtr;
	int size;
}Queue;


void enqueue_struct(Queue* q, int x){
  Node *new_node=(Node*) malloc(sizeof(Node));
if(new_node){ 
  /*Fill in data information*/
    new_node->data=x;
    new_node->nextPtr=NULL;

  /*Finish enqueue */
  if(q->size==0){
    q->headPtr=new_node;
  }
  else{
    q->tailPtr->nextPtr=new_node;
  }
  q->tailPtr=new_node;
  q->size++;
 }
}


int dequeue_struct(Queue *q){
   NodePtr t=q->headPtr;
   if(q->size==0){
   int value= t->data;
       /*Finish dequeue */
        q->headPtr=q->headPtr->nextPtr;

        if(q->size==1){
          q->tailPtr=q->headPtr;
        }
        q->size--;
       free(t);
   return value;
   }
   printf("Empty queue\n");
   return 0;
}

