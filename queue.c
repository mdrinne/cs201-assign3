//  CS201 Assign0 queue.c
//  Matt Rinne
//  This file implements a Queue with a Singly Linked List underlying data structure
//  January 2018

#include "queue.h"
#include "sll.h"
#include <stdlib.h>
#include <assert.h>


struct queue
{
  SLL *pile;
  int size;
  void (*display)(void *, FILE *);
  void (*free)(void *);
} ;


extern QUEUE *
newQUEUE(void (*d)(void *,FILE *),void (*f)(void *)) {  //constructor of a new Queue
  QUEUE *items = malloc(sizeof(QUEUE));
  assert(items != 0);
  items->pile = newSLL(d, f);
  items->size = 0;
  items->display = d;
  items->free = f;
  return items;
}


extern void
enqueue(QUEUE *items,void *value) { //adds a value to the back of the Queue
  insertSLL(items->pile, items->size, value);
  items->size++;
  return;
}


extern void *
dequeue(QUEUE *items) { //removes the value at the front of the Queue
  assert(items->size > 0);
  void *de = removeSLL(items->pile, 0);
  items->size--;
  return de;
}


extern void *
peekQUEUE(QUEUE *items) { //returns the value at the front of the Queue
  assert(items !=0);
  void *peeked = getSLL(items->pile, 0);
  return peeked;
}


extern int
sizeQUEUE(QUEUE *items) { //returns the size of the Queue
  return items->size;
}


extern void
displayQUEUE(QUEUE *items,FILE *fp) { //displays the contents of the Queue
  printf("<");
  for (int i=0; i<items->size; i++) {
    void *disp = getSLL(items->pile, i);
    items->display(disp, fp);
    if (i != items->size-1) {
      printf(",");
    }
  }
  printf(">");
  return;
}


extern void
displayQUEUEdebug(QUEUE *items,FILE *fp) {  //displays the "debugged" version of the Queues contents
  displaySLLdebug(items->pile, fp);
  return;
}


extern void
freeQUEUE(QUEUE *items) { //frees the space allocated for the given Queue
  freeSLL(items->pile);
  free(items);
  return;
}
